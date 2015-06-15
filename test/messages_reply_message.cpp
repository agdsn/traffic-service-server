#include "gtest/gtest.h"

#include "reply_message.h"
#include "replies.pb.h"


// Test that basic message serializing work
TEST(MessagesReply, basic_serialize_reply) {
	std::string bytes;
	traffic::ReplyMessage msg;

	msg.serialize(bytes);
	ASSERT_TRUE(bytes.size() != 0U);

	replies::Reply rep;
	rep.ParseFromString(bytes);

	EXPECT_EQ(rep.Payload_case(), replies::Reply::PAYLOAD_NOT_SET);
	EXPECT_EQ(rep.version(), 1U);
}


// Test Summary message generation
TEST(MessagesReply, summary_reply_message) {
	std::string bytes;
	traffic::SummaryReply msg;

	msg.add_entry("1.1.1.1", 10U, 20U);
	msg.add_entry("1.1.1.2", 30U, 40U);

	msg.serialize(bytes);
	ASSERT_TRUE(bytes.size() != 0U);

	replies::Reply rep;
	rep.ParseFromString(bytes);

	ASSERT_EQ(rep.Payload_case(), replies::Reply::kSummary);
	replies::Summary const &sum(rep.summary());
	EXPECT_EQ(sum.data_size(), 2);

	replies::SummaryEntry const &en1(sum.data(0));
	EXPECT_EQ(en1.address(), "1.1.1.1");
	EXPECT_EQ(en1.sum_traffic_in(), 10U);
	EXPECT_EQ(en1.sum_traffic_out(), 20U);

	replies::SummaryEntry const &en2(sum.data(1));
	EXPECT_EQ(en2.address(), "1.1.1.2");
	EXPECT_EQ(en2.sum_traffic_in(), 30U);
	EXPECT_EQ(en2.sum_traffic_out(), 40U);
}


// test Statistic message generation
TEST(MessagesReply, statistic_reply_test) {
	std::string bytes;
	traffic::StatisticReply msg(1U, 2U, 3U);

	msg.add_slice(10U, 11U, 12U);
	msg.add_slice(20U, 21U, 22U);

	msg.serialize(bytes);

	replies::Reply rep;
	rep.ParseFromString(bytes);

	ASSERT_EQ(rep.Payload_case(), replies::Reply::kStatistic);
	replies::Statistic const &stat(rep.statistic());
	EXPECT_EQ(stat.slice_size(), 2);

	EXPECT_EQ(stat.data_interval_step(), 1U);
	EXPECT_EQ(stat.sum_traffic_in(), 2U);
	EXPECT_EQ(stat.sum_traffic_out(), 3U);

	replies::StatisticSlice const &sl1(stat.slice(0));
	EXPECT_EQ(sl1.timestamp(), 10U);
	EXPECT_EQ(sl1.traffic_in(), 11U);
	EXPECT_EQ(sl1.traffic_out(), 12U);

	replies::StatisticSlice const &sl2(stat.slice(1));
	EXPECT_EQ(sl2.timestamp(), 20U);
	EXPECT_EQ(sl2.traffic_in(), 21U);
	EXPECT_EQ(sl2.traffic_out(), 22U);
}


// test Statistic message range generation
TEST(MessagesReply, statistic_reply_range_test) {
	std::string bytes;

	{
		traffic::StatisticReply msg(1U, 2U, 3U);

		msg.serialize(bytes);

		replies::Reply rep;
		rep.ParseFromString(bytes);

		ASSERT_EQ(rep.Payload_case(), replies::Reply::kStatistic);
		replies::Statistic const &stat(rep.statistic());

		EXPECT_FALSE(stat.has_available_range());
	}
	{
		traffic::StatisticReply msg(1U, 2U, 3U);
		msg.available_interval(traffic::TimeRange(2U, 1U));

		msg.serialize(bytes);

		replies::Reply rep;
		rep.ParseFromString(bytes);

		ASSERT_EQ(rep.Payload_case(), replies::Reply::kStatistic);
		replies::Statistic const &stat(rep.statistic());

		EXPECT_FALSE(stat.has_available_range());
	}
	{
		traffic::StatisticReply msg(1U, 2U, 3U);
		msg.available_interval(traffic::TimeRange(2U, 3U));

		msg.serialize(bytes);

		replies::Reply rep;
		rep.ParseFromString(bytes);

		ASSERT_EQ(rep.Payload_case(), replies::Reply::kStatistic);
		replies::Statistic const &stat(rep.statistic());

		EXPECT_TRUE(stat.has_available_range());
		EXPECT_EQ(stat.available_range().start(), 2U);
		EXPECT_EQ(stat.available_range().end(), 3U);
	}
}


// test error message generation
TEST(MessagesReply, error_message_test) {
	std::string bytes;
	traffic::ErrorReply msg(2U, "TestPeng");

	msg.serialize(bytes);

	replies::Reply rep;
	rep.ParseFromString(bytes);

	ASSERT_EQ(rep.Payload_case(), replies::Reply::kError);
	replies::Error const &err(rep.error());

	EXPECT_EQ(err.code(), 2U);
	EXPECT_EQ(err.reason(), "TestPeng");
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
