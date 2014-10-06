
#include <boost/assert.hpp>

#include "worker.h"
#include "requests.pb.h"


bool traffic::MessageWorker::process(std::string &result, void *data, size_t size)
{
	(void) result;

	request::Request request;
	request.ParseFromArray(data, size);

	switch (request.Payload_case()) {
		case request::Request::kStatistic:
			return process_statistics();
		case request::Request::kSummary:
			return process_summary();
		case request::Request::PAYLOAD_NOT_SET:
			return false;
	}

	BOOST_ASSERT(false && "Message parsing failed!");

	return false;
}
