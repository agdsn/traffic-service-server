#ifndef __BACKEND__SQLITE_H__
#define __BACKEND__SQLITE_H__

#include "backend.h"

typedef struct sqlite3 sqlite3;

namespace traffic {


class SqliteDataProvider : public DataProvider
{
	std::string const _dbname;
	sqlite3* _db;
	std::string const _inbound_table;
	std::string const _outbound_table;

	std::string _summary_query(size_t hostcount);

public:
	ReplyMessage fetch_summary(SummaryRequest const &request);
	ReplyMessage fetch_statistic(StatisticRequest const &request);

	bool valid() const;

	SqliteDataProvider(std::string const &dbname,
	                   std::string const &inbound_table,
	                   std::string const &outbound_table);

	~SqliteDataProvider();
};

class SqliteDataProviderFactory : public DataProviderFactory
{
	std::string const _dbname;
	std::string const _inbound_table;
	std::string const _outbound_table;
	static thread_local std::shared_ptr<SqliteDataProvider> _instance;
public:
	DataProvider::ptr_t instance();

	SqliteDataProviderFactory(std::string const &dbname,
	                          std::string const &inbound_table,
	                          std::string const &outbound_table);

	~SqliteDataProviderFactory();
};


}

#endif
