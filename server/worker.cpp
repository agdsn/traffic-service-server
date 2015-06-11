
#include <boost/assert.hpp>

#include "worker.h"
#include "session_context.h"
#include "requests.pb.h"


bool traffic::MessageWorker::process(std::string &result, void *data, size_t size) {
	SessionContext context(_factory.instance());
	context.encode_result(result);
	if (context.process_data(data, size)) {
		context.encode_result(result);
		return true;
	}
	BOOST_ASSERT(false && "Message processing failed!");

	return false;
}

bool traffic::MessageWorker::set_up() {
	if (_factory.instance())
		return true;
	BOOST_ASSERT(false && "Cannot initialize database");
	return false;
}
