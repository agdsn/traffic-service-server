#include "session_context.h"

#include <boost/assert.hpp>


bool traffic::SessionContext::process_data(void const *data, size_t const size)
{
	RequestMessage::ptr_t message(RequestMessage::parse_message(data, size));

	BOOST_ASSERT(message && "Parsing failed");

	try {
		message->accept(*this);
		return true;
	} catch (std::runtime_error ex) {
		BOOST_ASSERT(false && "Exception while message processing");
		return false;
	}
}

void traffic::SessionContext::encode_result(std::string &out)
{
	BOOST_ASSERT(_message && "No message set");
	_message->serialize(out);
}

traffic::SessionContext::SessionContext(DataProvider::ptr_t provider)
:
	_message(new ErrorReply(1, "No data processed")),
	_data_provider(provider)
{ }

void traffic::SessionContext::visit(const traffic::StatisticRequest &request) {
	_message.reset(new ReplyMessage(std::move(_data_provider->fetch_statistic(request))));
}

void traffic::SessionContext::visit(const traffic::SummaryRequest &request) {
	_message.reset(new ReplyMessage(std::move(_data_provider->fetch_summary(request))));
}

void traffic::SessionContext::visit(const traffic::ErrorRequest &) {
	_message.reset(new ErrorReply(3, "Cannot parse the message"));
}
