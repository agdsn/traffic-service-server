#include "message_context.h"

#include <boost/assert.hpp>


bool traffic::MessageContext::process_data(void const *data, size_t const size)
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

void traffic::MessageContext::encode_result(std::string &out)
{
	BOOST_ASSERT(_message && "No message set");
	_message->serialize(out);
}

traffic::MessageContext::MessageContext()
:
	_message(new ErrorReply(1, "No data processed"))
{ }

void traffic::MessageContext::visit(const traffic::StatisticRequest &request) {
    ReplyMessage::ptr_t reply(fetch_statistic(request));
    if (reply) {
        _message.swap(reply);
    }
}

void traffic::MessageContext::visit(const traffic::SummaryRequest &request) {
    ReplyMessage::ptr_t reply(fetch_summary(request));
    if (reply) {
        _message.swap(reply);
    }
}

void traffic::MessageContext::visit(const traffic::ErrorRequest &) {
	_message.reset(new ErrorReply(3, "Cannot parse the message"));
}
