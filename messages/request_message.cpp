#include <iostream>
#include <boost/assert.hpp>

#include "requests.pb.h"

#include "request_message.h"

namespace {
traffic::StatisticRequest::Interval convert_interval(requests::Statistic_DataInterval const &origin) {
	using Origin = requests::Statistic_DataInterval;
	using Traffic = traffic::StatisticRequest::Interval;

	switch (origin) {
		case Origin::Statistic_DataInterval_DAY:
			return Traffic::DAY;
		case Origin::Statistic_DataInterval_HOUR:
			return Traffic::HOUR;
		case Origin::Statistic_DataInterval_DETAIL:
			return Traffic::DETAIL;
	}

	BOOST_ASSERT(false);
	return Traffic::DETAIL;
}
}

traffic::RequestMessage::ptr_t
traffic::RequestMessage::parse_message(void const *data, size_t const size)
{
	requests::Request request;
	request.ParseFromArray(data, size);

	switch (request.Payload_case()) {
		case requests::Request::kStatistic:
			return ptr_t(new StatisticRequest(
						request.statistic()));
		case requests::Request::kSummary:
			return ptr_t(new SummaryRequest(
						request.summary()));
		case requests::Request::PAYLOAD_NOT_SET:
			std::cerr << "Payload not set or valid" << std::endl;
			break;
	}

	return ptr_t(new ErrorRequest());
}

traffic::StatisticRequest::StatisticRequest(requests::Statistic const &msg)
:
	_range(msg.range().start(), msg.range().end()),
	_address(msg.address()),
	_interval(convert_interval(msg.data_interval()))
{ }

traffic::SummaryRequest::SummaryRequest(requests::Summary const &msg)
:
	_range(msg.range().start(), msg.range().end()),
	_addresses(msg.addresses().begin(), msg.addresses().end())
{ }

void traffic::StatisticRequest::accept(traffic::RequestVisitor &visitor)
{
	visitor.visit(*this);
}

void traffic::SummaryRequest::accept(traffic::RequestVisitor &visitor)
{
	visitor.visit(*this);
}

void traffic::ErrorRequest::accept(traffic::RequestVisitor &visitor)
{
	visitor.visit(*this);
}
