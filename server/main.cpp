#include <iostream>
#include <string>

#include "commandline.h"
#include "traffic_server.h"
#include "worker.h"
#include "dummy.h"


int main(int argc, const char * argv[])
{
	std::cout << "Hi, I'm the traffic service" << std::endl;

	traffic::Commandline cmd;
	if (! cmd.parse(argc, argv)) {
		return 1;
	}

	traffic::TrafficServer server;

	for (std::string const &address : cmd.addresses()) {
		server.bind("tcp://" + address);
	}

	traffic::DummyProviderFactory fac;
	for (unsigned i = 0; i < cmd.worker(); ++i) {
		server.start_worker(new traffic::MessageWorker(fac));
	}

	server.run();

	return 0;
}

