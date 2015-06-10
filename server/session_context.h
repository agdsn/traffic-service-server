#ifndef __MESSAGES__MESSAGE_CONTEXT_H__
#define __MESSAGES__MESSAGE_CONTEXT_H__

#include "request_message.h"
#include "reply_message.h"
#include "backend.h"

namespace traffic {


class SessionContext : public RequestVisitor
{
private:
	std::unique_ptr<ReplyMessage> _message;
	DataProvider& _data_provider;

protected:
	void visit(StatisticRequest const &request);
	void visit(SummaryRequest const &request);
	void visit(ErrorRequest const &request);

public:
	bool process_data(void const *data, size_t const size);
	void encode_result(std::string &out);

	SessionContext(DataProvider& provider);
	virtual ~SessionContext() { }
};


}

#endif

