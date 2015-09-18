#ifndef __BACKEND__MYSQL_H__
#define __BACKEND__MYSQL_H__

#include <backend.h>


typedef struct session session;


namespace traffic {


class MySqlDataProvider : public DataProvider
{

	session * _sql;

public:

	ReplyMessage fetch_summary(SummaryRequest const &request)
	{
		(void) request;
		return ReplyMessage();
	}

	ReplyMessage fetch_statistic(StatisticRequest const &request)
	{
		(void) request;
		return ReplyMessage();
	}

	MySqlDataProvider(std::string const &host,
			  unsigned int port,
			  std::string const &user,
			  std::string const &password,
			  std::string const &database)
	{
		(void)host;
		(void)port;
		(void)user;
		(void)password;
		(void)database;
	}
};


class MySqlDataProviderFactory : public DataProviderFactory
{
public:
	DataProvider::ptr_t instance() { return 0; }

	MySqlDataProviderFactory(std::string const &host,
				 unsigned int port,
				 std::string const &user,
				 std::string const &password,
				 std::string const &database)
	{
		(void)host;
		(void)port;
		(void)user;
		(void)password;
		(void)database;
	}
};

}

#endif
