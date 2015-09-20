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


std::string traffic::Commandline::table_incomming() const
{
	BOOST_ASSERT(_vm.count("table_in") && "No incomming table specified!");

	if (! _vm.count("table_in"))
		return "";
	return _vm["table_in"].as<std::string>();
}


std::string traffic::Commandline::table_outgoing() const
{
	BOOST_ASSERT(_vm.count("table_out") && "No outgoing table specified!");

	if (! _vm.count("table_out"))
		return "";
	return _vm["table_out"].as<std::string>();
}


std::string traffic::Commandline::sqlite_file() const
{
	BOOST_ASSERT(_vm.count("sqlite_file") && "No sqlite file specified!");

	if (! _vm.count("sqlite_file"))
		return "";
	return _vm["sqlite_file"].as<std::string>();
}


std::string traffic::Commandline::host() const
{
	BOOST_ASSERT(_vm.count("host") && "No host specified!");

	if (! _vm.count("host"))
		return "";
	return _vm["host"].as<std::string>();
}


unsigned int traffic::Commandline::port() const
{
	BOOST_ASSERT(_vm.count("port") && "No port specified!");

	if (! _vm.count("port"))
		return 0;
	return _vm["port"].as<unsigned int>();
}


std::string traffic::Commandline::user() const
{
	BOOST_ASSERT(_vm.count("user") && "No user specified!");

	if (! _vm.count("user"))
		return "";
	return _vm["user"].as<std::string>();
}


std::string traffic::Commandline::password() const
{
	BOOST_ASSERT(_vm.count("password") && "No password specified!");

	if (! _vm.count("password"))
		return "";
	return _vm["password"].as<std::string>();
}


std::string traffic::Commandline::database() const
{
	BOOST_ASSERT(_vm.count("database") && "No database specified!");

	if (! _vm.count("database"))
		return "";
	return _vm["database"].as<std::string>();
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

	switch (storage_type()) {
		case SQLITE:
			if (!_vm.count("sqlite_file")) {
				std::cerr << "option --sqlite_file missing for "
					     "storage_type sqlite!"
					  << std::endl;
				return false;
			}
			break;
		case MYSQL:
			if (!_vm.count("host")) {
				std::cerr << "option --host missing for "
					     "storage_type mysql!"
					  << std::endl;
				return false;
			}
			if (!_vm.count("user")) {
				std::cerr << "option --user missing for "
					     "storage_type mysql!"
					  << std::endl;
				return false;
			}
			if (!_vm.count("password")) {
				std::cerr << "option --password missing for "
					     "storage_type mysql!"
					  << std::endl;
				return false;
			}
			if (!_vm.count("database")) {
				std::cerr << "option --database missing for "
					     "storage_type mysql!"
					  << std::endl;
				return false;
			}
			break;
		case POSTGRES:
			break;
	}

	return true;
}


traffic::Commandline::Commandline()
:
	_desc("Traffic query server options"),
	_vm()
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

	po::options_description mysql("MySql options");
	mysql.add_options()
		("host", po::value<std::string>(),
		 "Database host")
		("port", po::value<unsigned int>()->default_value(3306),
		 "Database port")
		("user", po::value<std::string>(),
		 "Database user")
		("password", po::value<std::string>(),
		 "Database password")
		("database", po::value<std::string>(),
		 "Database name")
		;

	_desc.add(general).add(sqlite).add(mysql);
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
