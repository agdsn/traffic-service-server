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

	bool process(std::string& res, void*, size_t)
	{
		message = true;
		res += "XXX";
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

	usleep(30); // Wari for the proxy() call

	{
		zmq::context_t ctx(1);
		zmq::socket_t sock(ctx, ZMQ_REQ);
		sock.connect(TEST_ADDRESS);

		zmq::message_t msg(10);
		EXPECT_TRUE(sock.send(msg));

		zmq::message_t res;
		EXPECT_TRUE(sock.recv(&res));
		EXPECT_EQ((size_t)3U, res.size());

		sock.close();
	}

	server.stop();
	tr.join();
	EXPECT_TRUE(w.message);
}
