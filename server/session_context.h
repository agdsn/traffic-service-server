#ifndef __MESSAGES__MESSAGE_CONTEXT_H__
#define __MESSAGES__MESSAGE_CONTEXT_H__

#include "request_message.h"
#include "reply_message.h"
#include "backend.h"

namespace traffic {


/**
 * \brief This is the context for a request/reply session.
 *
 * This context is allocated for each request to process the data. It represents
 * the glue between the server, the messages and the backend. It uses the
 * RequestMessage interface to parse data to a request, implements the
 * RequestVisitor interface to select the correct backend method to query and
 * provide a interface to serialize the result to the wire format.
 */
class SessionContext : public RequestVisitor
{
private:
	std::unique_ptr<ReplyMessage> _message;
	DataProvider::ptr_t _data_provider;

protected:
	void visit(StatisticRequest const &request);
	void visit(SummaryRequest const &request);
	void visit(ErrorRequest const &request);

public:
	/**
	 * \brief Process raw request data in wire format.
	 *
	 * This is the entry point for the context to process its request. It
	 * gets the wire data of the request, transforms it to a request
	 * instance and give it to the backend. The result from the backend will
	 * be stored internally.
	 *
	 * \param data The pointer to the raw request data.
	 * \param size The size of the request data.
	 * \return false in case of error.
	 */
	bool process_data(void const *data, size_t const size);

	/**
	 * \brief Encode the current reply data to the wire format.
	 *
	 * This serialize the current state (result from the backend or error
	 * message) to the reply wire format and put the result into the given
	 * string reference.
	 *
	 * \param out The string to write the result to.
	 */
	void encode_result(std::string &out);

	/**
	 * \brief Creat a session context.
	 *
	 * This is done for every incomming data packet.
	 *
	 * \param provider The DataProvider to use for this request.
	 */
	SessionContext(DataProvider::ptr_t provider);
	virtual ~SessionContext() { }
};


}

#endif

