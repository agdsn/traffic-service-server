#include <istream>
#include <iostream>

#include <boost/assert.hpp>

#include "commandline.h"


#define DEFAULT_CONNECTION "*:3444"
#define DEFAULT_WORKER     2U


namespace traffic {
	std::istream& operator>>(std::istream& in,
				 Commandline::StorageType& type);
}


std::vector<std::string>
traffic::Commandline::addresses() const
{
	BOOST_ASSERT(_vm.count("bind") && "No address there to bind!");

	if (! _vm.count("bind"))
		return std::vector<std::string>(1, DEFAULT_CONNECTION);
	return _vm["bind"].as<std::vector<std::string> >();
}


unsigned int
traffic::Commandline::worker() const
{
	BOOST_ASSERT(_vm.count("worker") && "No worker count specified");

	if (! _vm.count("worker"))
		return DEFAULT_WORKER;
	return _vm["worker"].as<unsigned int>();
}


traffic::Commandline::StorageType
traffic::Commandline::storage_type() const
{
	BOOST_ASSERT(_vm.count("storage") && "No storage type specified!");

	if (! _vm.count("storage"))
		return SQLITE;
	return _vm["storage"].as<StorageType>();
}


bool traffic::Commandline::parse(int argc, char const *argv[])
{
	try {
		po::store(po::parse_command_line(argc, argv, _desc), _vm);

		if (_vm.count("help")) {
			std::cout << _desc << std::endl;
			return false;
		}

		po::notify(_vm);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}


traffic::Commandline::Commandline()
:
	_desc("Traffic query server options")
{
	po::options_description general("General options");
	general.add_options()
		("help,h", "Print help message.")
		("bind,b",
		 po::value<std::vector<std::string> >()
			->default_value(
				std::vector<std::string>(1,
					DEFAULT_CONNECTION),
				DEFAULT_CONNECTION)
			->composing(),
		 "address:port combinations to bind to")
		("worker,w", po::value<unsigned int>()->default_value(2),
		 "Number of workers to spawn")
		("storage,s", po::value<StorageType>()->required(),
		 "Specify the backend storage type (sqlite, mysql, postgres)")
		("table_in,I", po::value<std::string>()->required(),
		 "The database table name for incomming traffic")
		("table_out,O", po::value<std::string>()->required(),
		 "The database table name for outgoing traffic")
		;

	po::options_description sqlite("SqlLite options");
	sqlite.add_options()
		("sqlite_file", po::value<std::string>(),
		 "Database file")
		;

	_desc.add(general).add(sqlite);
}


std::istream& traffic::operator>>(std::istream& in,
		                  Commandline::StorageType& type)
{
	std::string token;
	in >> token;
	if (token == "sqlite")
		type = traffic::Commandline::SQLITE;
	else if (token == "mysql")
		type = traffic::Commandline::MYSQL;
	else if (token == "postgres")
		type = traffic::Commandline::POSTGRES;
	else throw po::validation_error(
			po::validation_error::invalid_option_value);
	return in;
}
