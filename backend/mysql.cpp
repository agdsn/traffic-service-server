#include "mysql.h"

#include <sstream>
#include <soci/soci.h>



std::string connection_string(std::string const &host,
			      unsigned int port,
			      std::string const &user,
			      std::string const &password,
			      std::string const &database)
{
	std::stringstream conn;
	conn << "mysql://host='" << host << "' port=" << port
		<< " user='" << user << "' password='" << password << "'"
		<< " database='" << database << "'";
	return conn.str();
}


traffic::MySqlDataProvider::MySqlDataProvider(std::string const &host,
					      unsigned int port,
					      std::string const &user,
					      std::string const &password,
					      std::string const &database)
:
	_sql_ptr(new soci::session())
{
	_sql_ptr->open(connection_string(host, port,
				user, password, database));
}


traffic::MySqlDataProvider::~MySqlDataProvider()
{
	_sql_ptr->close();
	delete _sql_ptr;
}


traffic::DataProvider::ptr_t traffic::MySqlDataProviderFactory::instance()
{
	if (! _instance) {
		_instance.reset(new MySqlDataProvider(_host, _port,
					_user, _password, _database));
	}
	return _instance;
}


traffic::MySqlDataProviderFactory::MySqlDataProviderFactory(
		std::string const &host,
		unsigned int port,
		std::string const &user,
		std::string const &password,
		std::string const &database)
:
	_host(host),
	_port(port),
	_user(user),
	_password(password),
	_database(database)
{ }

thread_local std::shared_ptr<traffic::MySqlDataProvider>
	traffic::MySqlDataProviderFactory::_instance;
