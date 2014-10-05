#ifndef __SERVER__WORKER_H__
#define __SERVER__WORKER_H__


#include "traffic_server.h"


namespace traffic {

class MessageWorker : public TrafficServer::WorkerBase {


protected:
	bool process(std::string &result, void * request, size_t size);
	virtual bool process_statistics() = 0;
	virtual bool process_summary() = 0;

public:
	MessageWorker() { }
	~MessageWorker() { }

};

}

#endif
