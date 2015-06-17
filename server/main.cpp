#include <iostream>
#include <string>

#include "commandline.h"
#include "traffic_server.h"
#include "worker.h"
#include "dummy.h"
#include "sqlite.h"


traffic::DataProviderFactory *create_factory(traffic::Commandline const &cmd)
{
	switch (cmd.storage_type()) {
		case traffic::Commandline::SQLITE:
			return new traffic::SqliteDataProviderFactory(cmd.sqlite_file(),
					cmd.table_incomming(),
					cmd.table_outgoing());
		default:
			return new traffic::DummyProviderFactory();
	}
}

int main(int argc, const char * argv[])
{
	std::cout << "Hi, I'm the traffic service" << std::endl;

	traffic::Commandline cmd;
	if (! cmd.parse(argc, argv)) {
		return 1;
	}

	traffic::TrafficServer server;

	for (std::string const &address : cmd.addresses()) {
		std::cout << "Bind server to " << address << std::endl;
		server.bind("tcp://" + address);
	}


	std::unique_ptr<traffic::DataProviderFactory> fac(create_factory(cmd));

	for (unsigned i = 0; i < cmd.worker(); ++i) {
		server.start_worker(new traffic::MessageWorker(*fac));
	}

	server.run();

	return 0;
}

