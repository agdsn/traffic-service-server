#ifndef __SERVER__WORKER_H__
#define __SERVER__WORKER_H__


#include <backend.h>
#include "traffic_server.h"


namespace traffic {

class MessageWorker : public TrafficServer::WorkerBase {

	DataProviderFactory &_factory;

protected:
	bool process(std::string &result, void * request, size_t size);
	bool set_up();

public:
	MessageWorker(DataProviderFactory & factory) : _factory(factory) { }
	~MessageWorker() { }

};

}

#endif
