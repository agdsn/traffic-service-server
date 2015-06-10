#ifndef __MESSAGES__MESSAGE_CONTEXT_H__
#define __MESSAGES__MESSAGE_CONTEXT_H__

#include "request_message.h"
#include "reply_message.h"

namespace traffic {


class MessageContext : public RequestVisitor
{
private:
	std::unique_ptr<ReplyMessage> _message;


protected:
	void visit(StatisticRequest const &request);
	void visit(SummaryRequest const &request);
	void visit(ErrorRequest const &request);

    virtual ReplyMessage::ptr_t&& fetch_summary(SummaryRequest const &request) = 0;
	virtual ReplyMessage::ptr_t&& fetch_statistic(StatisticRequest const &request) = 0;

public:
	bool process_data(void const *data, size_t const size);
	void encode_result(std::string &out);

	MessageContext();
	virtual ~MessageContext() { }
};


}

#endif

