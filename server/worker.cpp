
#include <boost/assert.hpp>

#include "worker.h"
#include "requests.pb.h"


bool traffic::MessageWorker::process(std::string &result, void *data, size_t size)
{
	(void) result;

	request::Request request;
	request.ParseFromArray(data, size);

	if (request.has_statistic())
		return process_statistics();
	else if (request.has_summary())
		return process_summary();

	BOOST_ASSERT(false && "Unknown message type");

	return false;
}
