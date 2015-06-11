#include <iostream>

#include "replies.pb.h"
#include "reply_message.h"

void traffic::ReplyMessage::serialize(std::string &output) const
{
	_reply_msg->SerializeToString(&output);
}


traffic::ReplyMessage::ReplyMessage()
:
	_reply_msg(new replies::Reply())
{
	_reply_msg->set_version(1U);
}


void
traffic::SummaryReply::add_entry(std::string const &address,
				 traffic_t in, traffic_t out)
{
	replies::SummaryEntry * entry(_summary_msg->add_data());
	entry->set_address(address);
	entry->set_sum_traffic_in(in);
	entry->set_sum_traffic_out(out);
}


traffic::SummaryReply::SummaryReply()
:
	_summary_msg(_reply_msg->mutable_summary())
{ }


void
traffic::StatisticReply::add_slice(time_t timestamp,
				   traffic_t in, traffic_t out)
{
	replies::StatisticSlice * slice(_statistic_msg->add_slice());
	slice->set_timestamp(timestamp);
	slice->set_traffic_in(in);
	slice->set_traffic_out(out);
}


void
traffic::StatisticReply::available_interval(TimeRange const &range)
{
	if (! range.valid()) {
		std::cerr << "Invalid TimeRange - ignore it." << std::endl;
		return;
	}

	common::TimeRange * range_msg(
			_statistic_msg->mutable_available_range());
	range_msg->set_start(range.start());
	range_msg->set_end(range.end());
}


traffic::StatisticReply::StatisticReply(time_t interval,
					traffic_t in, traffic_t out)
:
	_statistic_msg(_reply_msg->mutable_statistic())
{
	_statistic_msg->set_data_interval_step(interval);
	_statistic_msg->set_sum_traffic_in(in);
	_statistic_msg->set_sum_traffic_out(out);
}


traffic::ErrorReply::ErrorReply(uint32_t code, std::string const &msg)
:
	_error_msg(_reply_msg->mutable_error())
{
	_error_msg->set_code(code);
	_error_msg->set_reason(msg);
}

traffic::ReplyMessage::~ReplyMessage() { }

traffic::ReplyMessage::ReplyMessage(traffic::ReplyMessage &&message)
:
	_reply_msg(message._reply_msg.release())
{ }

traffic::SummaryReply::SummaryReply(traffic::SummaryReply &&reply)
:
	ReplyMessage(std::move(reply)),
	_summary_msg(std::move(reply._summary_msg))
{
	reply._summary_msg = 0;
}

traffic::StatisticReply::StatisticReply(traffic::StatisticReply &&reply)
:
	ReplyMessage(std::move(reply)),
	_statistic_msg(std::move(reply._statistic_msg))
{
	reply._statistic_msg = 0;
}

traffic::ErrorReply::ErrorReply(traffic::ErrorReply &&reply)
:
	ReplyMessage(std::move(reply)),
	_error_msg(std::move(reply._error_msg))
{
	_error_msg = 0;
}
