#ifndef __BACKEND__MYSQL_H__
#define __BACKEND__MYSQL_H__

#include "backend.h"

namespace soci {
	typedef struct session session;
}


namespace traffic {


class MySqlDataProvider : public DataProvider
{

	soci::session * _sql_ptr;
	void ensure_connection();

public:

	ReplyMessage fetch_summary(SummaryRequest const &request);

	ReplyMessage fetch_statistic(StatisticRequest const &request);

	MySqlDataProvider(std::string const &host,
			  unsigned int port,
			  std::string const &user,
			  std::string const &password,
			  std::string const &database);

	~MySqlDataProvider();
};


class MySqlDataProviderFactory : public DataProviderFactory
{
	std::string const &_host;
	unsigned int _port;
	std::string const &_user;
	std::string const &_password;
	std::string const &_database;

	static thread_local std::shared_ptr<MySqlDataProvider> _instance;

public:
	DataProvider::ptr_t instance();

	MySqlDataProviderFactory(std::string const &host,
				 unsigned int port,
				 std::string const &user,
				 std::string const &password,
				 std::string const &database);
};

}

#endif
