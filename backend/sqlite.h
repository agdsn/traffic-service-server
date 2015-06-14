#ifndef __BACKEND__SQLITE_H__
#define __BACKEND__SQLITE_H__

#include <sqlite3.h>

#include "backend.h"

typedef struct sqlite3 sqlite3;

namespace traffic {


class SqliteDataProvider : public DataProvider
{
    std::string const _dbname;
    sqlite3* _db;

public:
    ReplyMessage fetch_summary(SummaryRequest const &request);
	ReplyMessage fetch_statistic(StatisticRequest const &request);

    bool valid() const;

    SqliteDataProvider(std::string const &dbname);
};

class SqliteDataProviderFactory : public DataProviderFactory
{
    std::string const _dbname;
    static thread_local std::shared_ptr<SqliteDataProvider> _instance;
public:
    DataProvider::ptr_t instance();

    SqliteDataProviderFactory(std::string const &dbname);
};


}

#endif
