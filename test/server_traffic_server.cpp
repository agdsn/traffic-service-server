#include <algorithm>
#include <thread>
#include <unistd.h>
#include <zmq.hpp>

#include "gtest/gtest.h"
#include "traffic_server.h"


#define TEST_ADDRESS "tcp://127.0.0.2:11100"


class TestWorker : public traffic::TrafficServer::WorkerBase
{
public:
	std::mutex _m;
	std::condition_variable _cv;

	bool started = false;
	bool message = false;

	bool set_up()
	{
		started = true;
		return true;
	}

	bool process(std::string& res, void*, size_t sz)
	{
		message = true;
		res = std::string(sz, 'X');
		return true;
	}

	~TestWorker() { }
};


// Test if the basic worker communication work.
TEST(ServerTrafficServer, test_bind_worker) {
	TestWorker w;
	traffic::TrafficServer server;
	server.bind(TEST_ADDRESS);

	server.start_worker( &w );
	EXPECT_TRUE(w.started);

	std::thread tr([ & ] (){ server.run(); });

	usleep(30); // Wait for the proxy() call

	{
		zmq::context_t ctx(1);
		zmq::socket_t sock(ctx, ZMQ_REQ);
		sock.connect(TEST_ADDRESS);

		zmq::message_t msg(10);
		EXPECT_TRUE(sock.send(msg));

		zmq::message_t res;
		EXPECT_TRUE(sock.recv(&res));
		EXPECT_EQ((size_t)10U, res.size());

		sock.close();
	}

	server.stop();
	tr.join();
	EXPECT_TRUE(w.message);
}


// Test multiple worker message distribution
TEST(ServerTrafficServer, test_multi_workers) {
	std::vector<TestWorker> workers(3);

	traffic::TrafficServer server;
	server.bind(TEST_ADDRESS);

	for (auto& worker : workers) {
		server.start_worker( &worker );
		EXPECT_TRUE(worker.started);
	}

	std::thread tr([ & ] (){ server.run(); });

	usleep(30); // Wait for the proxy() call

	for (size_t i = 0; i < workers.size(); ++i) {
		zmq::context_t ctx(1);
		zmq::socket_t sock(ctx, ZMQ_REQ);
		sock.connect(TEST_ADDRESS);

		zmq::message_t msg(10+i);
		EXPECT_TRUE(sock.send(msg));

		zmq::message_t res;
		EXPECT_TRUE(sock.recv(&res));
		EXPECT_EQ((size_t)(10U+i), res.size());

		sock.close();
		EXPECT_EQ((int)(i+1U),
			   std::count_if(workers.begin(),
					 workers.end(),
					 [](TestWorker const &w)
						{ return w.message; }));
	}

	server.stop();
	tr.join();
}
