#include "gtest/gtest.h"

#include "worker.h"
#include "backend.h"
#include "requests.pb.h"
#include "replies.pb.h"


class TestMessageWorker : public traffic::MessageWorker
{
protected:
	bool set_up() { return true; }

public:
	struct TestProviderFactory : public traffic::DataProviderFactory
	{
		bool statistic;
		bool summary;

		TestProviderFactory() : statistic(false), summary(false) { }

		traffic::DataProvider::ptr_t instance() { return traffic::DataProvider::ptr_t(new TestProvider(*this)); };
	};

	struct TestProvider : public traffic::DataProvider
	{
		TestProviderFactory & _tpf;

		TestProvider(TestProviderFactory & tpf) : _tpf(tpf) {}

		virtual traffic::ReplyMessage fetch_summary(traffic::SummaryRequest const &)
		{
			_tpf.summary = true;
			return std::move(traffic::ErrorReply(1, "PENG"));
		}

		traffic::ReplyMessage fetch_statistic(traffic::StatisticRequest const &)
		{
			_tpf.statistic = true;


			return traffic::ErrorReply(0, "TestError");
		}
	};

	using traffic::MessageWorker::process;

	TestMessageWorker(traffic::DataProviderFactory& pf)
	:
		traffic::MessageWorker(pf)
	{ }
};


TEST(ServerMessageWorker, basic_message_worker) {
	TestMessageWorker::TestProviderFactory pf;
	TestMessageWorker w(pf);

	requests::Request request;
	request.set_version(1);

	requests::Statistic *stat = request.mutable_statistic();

	common::TimeRange *range = stat->mutable_range();

	range->set_start(1);
	range->set_end(2);

	stat->set_address("10.10.10.10");

	std::string buffer;
	request.SerializeToString(&buffer);

	std::string out_buffer;
	w.process(out_buffer, (void *)buffer.c_str(), buffer.size());

	EXPECT_TRUE(pf.statistic);
	EXPECT_FALSE(pf.summary);

	replies::Reply reply;
	reply.ParseFromString(out_buffer);

	EXPECT_EQ(replies::Reply::kError, reply.Payload_case());
	EXPECT_EQ("TestError", reply.error().reason());
}
