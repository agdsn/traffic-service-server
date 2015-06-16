#include "gtest/gtest.h"

#include <typeinfo>

#include "request_message.h"
#include "requests.pb.h"



class TypeVisitor : public traffic::RequestVisitor
{
public:
	enum Type {
		SUMMARY,
		STATISTIC,
		ERROR,
		NOTSET
	};

private:
	Type _t;

protected:
	void visit(traffic::StatisticRequest const &) { _t = STATISTIC; }
	void visit(traffic::SummaryRequest const &) { _t = SUMMARY; }
	void visit(traffic::ErrorRequest const &) { _t = ERROR; }

public:
	TypeVisitor() : _t(NOTSET) { }

	Type visited() const { return _t; }
};


std::string summary_message()
{
	requests::Request req;
	req.set_version(1);

	requests::Summary *summary(req.mutable_summary());
	summary->mutable_range()->set_start(1UL);
	summary->mutable_range()->set_end(2UL);

	summary->add_addresses("peng1");
	summary->add_addresses("peng2");
	summary->add_addresses("peng3");
	summary->add_addresses("peng4");

	return req.SerializeAsString();
}


std::string statistic_message()
{
	requests::Request req;
	req.set_version(1);

	requests::Statistic *statistic(req.mutable_statistic());
	statistic->mutable_range()->set_start(1UL);
	statistic->mutable_range()->set_end(2UL);

	statistic->set_address("peng");
	statistic->set_data_interval(requests::Statistic::HOUR);

	return req.SerializeAsString();
}


// Test that summary message processing work
TEST(MessagesRequest, deserialize_summary_request) {
	std::string bytes(summary_message());

	traffic::RequestMessage::ptr_t msg(
			traffic::RequestMessage::parse_message(bytes.c_str(),
							       bytes.size()));


	ASSERT_EQ(typeid(traffic::SummaryRequest), typeid(*msg));

	TypeVisitor v;
	msg->accept(v);
	ASSERT_EQ(TypeVisitor::SUMMARY, v.visited());

	traffic::SummaryRequest* summary(dynamic_cast<traffic::SummaryRequest*>(msg.get()));

	ASSERT_NE(nullptr, summary);
	EXPECT_EQ(1, summary->range().start());
	EXPECT_EQ(2, summary->range().end());

	ASSERT_EQ(4U, summary->addresses().size());
	EXPECT_EQ("peng1", summary->addresses().at(0));
	EXPECT_EQ("peng2", summary->addresses().at(1));
	EXPECT_EQ("peng3", summary->addresses().at(2));
	EXPECT_EQ("peng4", summary->addresses().at(3));

}


// Test that statistic message processing work
TEST(MessagesRequest, deserialize_statistic_request) {
	std::string bytes(statistic_message());

	traffic::RequestMessage::ptr_t msg(
			traffic::RequestMessage::parse_message(bytes.c_str(),
							       bytes.size()));


	ASSERT_EQ(typeid(traffic::StatisticRequest), typeid(*msg));

	TypeVisitor v;
	msg->accept(v);
	ASSERT_EQ(TypeVisitor::STATISTIC, v.visited());

	traffic::StatisticRequest* stat(dynamic_cast<traffic::StatisticRequest*>(msg.get()));

	ASSERT_NE(nullptr, stat);
	EXPECT_EQ(1, stat->range().start());
	EXPECT_EQ(2, stat->range().end());

	EXPECT_EQ("peng", stat->address());
	EXPECT_EQ(traffic::StatisticRequest::HOUR, stat->interval());
}




/**
 * Shut down the protobuf library after the tests to make valgrind happy
 */
namespace {

class ShutdownEnvironment : public ::testing::Environment
{
public:
	void TearDown()
	{
		google::protobuf::ShutdownProtobufLibrary();
	}
};

#ifndef NDEBUG
::testing::Environment* const shutdown_env =
	::testing::AddGlobalTestEnvironment(new ShutdownEnvironment);
#endif
}
