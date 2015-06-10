#include <iostream>

#include <zmq.hpp>


#include "traffic_server.h"


void traffic::TrafficServer::WorkerBase::run(zmq::context_t &context)
{
	zmq::socket_t socket (context, ZMQ_REP);

	{
		std::lock_guard<std::mutex> lk(_setup_mutex);

		if (! this->set_up()) {
			std::cerr << "Error in worker setup!" << std::endl;
			_setup_done = true;
			return;
		}

		socket.connect (WORKER_PORT);
		_setup_done = true;
	}
	std::cerr << "setup done" << std::endl;
	_cv.notify_all();

	while (true) {
		try {
			zmq::message_t request;
			socket.recv (&request);

			std::string result;
			if (!this->process(result, request.data(), request.size()))
				std::cerr << "Error processing message of "
					  << request.size() << " bytes!";

			zmq::message_t response(result.size());
			memcpy(response.data(), result.c_str(), result.size());

			socket.send(response);
		} catch (std::exception &e) {
			std::cerr << "Worker died: " << e.what() << std::endl;
			break;
		}
	}
	socket.close();
}


traffic::TrafficServer::WorkerBase::WorkerBase()
:
	_setup_mutex(),
	_cv(),
	_setup_done(false)
{ }


traffic::TrafficServer::WorkerBase::~WorkerBase()
{ }


bool traffic::TrafficServer::bind(std::string const &address)
{
	try {
		_incomming->bind(address.c_str());
	} catch (zmq::error_t const & e) {
		std::cerr << "cannot bind to: '"
			  << address << "': "
		          << e.what() << std::endl;
		return false;
	}
	return true;
}


bool traffic::TrafficServer::start_worker(WorkerBase * worker)
{
	_threads.push_back(std::thread([&] () { worker->run(*_context); }));

	std::unique_lock<std::mutex> lk(worker->_setup_mutex);
	worker->_cv.wait(lk, [ & ] () { return worker->_setup_done; });
	lk.unlock();
	return true;
}


void traffic::TrafficServer::run()
{
	try {
		zmq::proxy(*_incomming, *_workers, NULL);
	} catch (zmq::error_t const &e) {
		std::cerr << "Cannot proxy between incomming and workers:"
			  << e.what() << std::endl;
	}
}


void traffic::TrafficServer::stop()
{
	_workers->close();
	_incomming->close();
	_context.reset();
	for (std::thread &thread : _threads)
		thread.join();
}


traffic::TrafficServer::TrafficServer()
:
	_context(new zmq::context_t(1)),
	_incomming(new zmq::socket_t(*_context, ZMQ_ROUTER)),
	_workers(new zmq::socket_t(*_context, ZMQ_DEALER))
{
	_workers->bind(WORKER_PORT);
}


traffic::TrafficServer::~TrafficServer() { }
