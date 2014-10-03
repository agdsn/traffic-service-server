#ifndef __SERVER__TRAFFIC_SERVER_H__
#define __SERVER__TRAFFIC_SERVER_H__

#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>


#define WORKER_PORT "inproc://workers"


namespace zmq {
	class context_t;
	class socket_t;
}


namespace traffic {


/**
 * \brief This handles the connections from the world.
 *
 * The TrafficServer handles the connections from the clients and proxy
 * them fairly to the workers. It is also responsible for the socket
 * setup and the creation of the worker threads.
 */
class TrafficServer
{
public:

	/**
	 * \brief This is the base for all workers.
	 *
	 * Implement this class in all worker implementations the
	 * server can handle. It implements the message receiving and
	 * sending.
	 */
	class WorkerBase
	{
	private:
		std::mutex _setup_mutex;
		std::condition_variable _cv;
		bool _setup_done;

		void run(zmq::context_t &context);

		WorkerBase(WorkerBase const &) = delete;
		WorkerBase&
		operator= (WorkerBase const &) = delete;

	protected:
		/**
		 * \brief Setup worker thread.
		 *
		 * Implement this to do the setup of the worker (e.g.
		 * connect to databases). It will be called after the
		 * thread was started.
		 *
		 * \return false in case of error.
		 */
		virtual bool set_up() = 0;

		/**
		 * \brief This is the "work" method.
		 *
		 * This method is called when a message was received.
		 * Implement it to du the actual work. The result
		 * should be stored in the result string.
		 *
		 * \param result The result of the processing.
		 * \param data   The data from the received message
		 * \param size   The size of the received message data.
		 * \return false in case of error.
		 */
		virtual bool process(std::string &result,
				     void * data,
				     size_t size) = 0;

		WorkerBase();
		virtual ~WorkerBase();

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

	/**
	 * \brief bind the Server to the given addrress.
	 *
	 * This tells zeroMQ to bind the client socket to the given
	 * address. The format of the address is specified in
	 * zmq_tcp(7). This must be called before calling the run()
	 * method.
	 *
	 * \param address The address to bind to.
	 */
	bool bind(std::string const &address);

	/**
	 * \brief Start a specific worker.
	 *
	 * This starts the given worker in its own thread. It calls the
	 * run() method of the Worker to start the work loop. This
	 * must be called befor the run() method of the Server.
	 *
	 * \param worker The worker to start.
	 */
	bool start_worker(WorkerBase * worker);

	/**
	 * \brief Start the server-IO.
	 *
	 * This starts the proxy between the incomming client
	 * connections and the workers. You should have all bindings
	 * done and workers running when calling this.
	 */
	void run();

	void stop();

	TrafficServer();
	~TrafficServer();
};

}

#endif
