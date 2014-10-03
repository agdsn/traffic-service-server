#ifndef __SERVER__TRAFFIC_SERVER_H__
#define __SERVER__TRAFFIC_SERVER_H__

#include <memory>
#include <string>
#include <thread>
#include <vector>


#define WORKER_PORT "inproc://workers"


namespace zmq {
	class context_t;
	class socket_t;
}


namespace traffic {

class TrafficServer
{
public:
	class WorkerBase
	{
	private:
		void run(zmq::context_t &context);

		WorkerBase(WorkerBase const &) = delete;
		WorkerBase&
		operator= (WorkerBase const &) = delete;

	protected:
		virtual bool process(std::string &result,
				     void * data,
				     size_t size) = 0;

		virtual ~WorkerBase() { }

		friend class TrafficServer;
	};

private:
	std::shared_ptr<zmq::context_t> _context;
	std::shared_ptr<zmq::socket_t> _incomming;
	std::shared_ptr<zmq::socket_t> _workers;
	std::vector<std::thread> _threads;

	TrafficServer(TrafficServer const &) = delete;
	TrafficServer& operator= (TrafficServer const &) = delete;

public:
	bool bind(std::string const &address);
	bool start_worker(WorkerBase * worker);

	void run();

	TrafficServer();
	~TrafficServer();
};

}

#endif
