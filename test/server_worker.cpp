#include "gtest/gtest.h"

#include "worker.h"
#include "requests.pb.h"


class TestMessageWorker : public traffic::MessageWorker
{
protected:
	bool set_up() { return true; }

	bool process_summary() {
		summary = true;
		return true;
	}

	bool process_statistics() {
		statistic = true;
		return true;
	}

public:
	bool statistic;
	bool summary;

	using traffic::MessageWorker::process;

	TestMessageWorker()
	:
		traffic::MessageWorker(),
		statistic(false),
		summary(false)
	{ }
};


TEST(ServerMessageWorker, basic_message_worker) {
	TestMessageWorker w;

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

	EXPECT_TRUE(w.statistic);
	EXPECT_FALSE(w.summary);
}
