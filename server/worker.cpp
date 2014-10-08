
#include <boost/assert.hpp>

#include "worker.h"
#include "requests.pb.h"


bool traffic::MessageWorker::process(std::string &result, void *data, size_t size)
{
	(void) result;

	requests::Request request;
	request.ParseFromArray(data, size);

	switch (request.Payload_case()) {
		case requests::Request::kStatistic:
			return process_statistics();
		case requests::Request::kSummary:
			return process_summary();
		case requests::Request::PAYLOAD_NOT_SET:
			return false;
	}

	BOOST_ASSERT(false && "Message parsing failed!");

	return false;
}
