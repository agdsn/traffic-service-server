#include "sqlite.h"

#include <sqlite3.h>
#include <iostream>

traffic::ReplyMessage traffic::SqliteDataProvider::fetch_summary(const traffic::SummaryRequest &request) {
    if (! valid())
        return ErrorReply(10, "No valid database connection");
    (void)request;
    return traffic::ReplyMessage();
}

traffic::ReplyMessage traffic::SqliteDataProvider::fetch_statistic(const traffic::StatisticRequest &request) {
    if (! valid())
        return ErrorReply(10, "No valid database connection");
    (void)request;
    return traffic::ReplyMessage();
}

traffic::SqliteDataProvider::SqliteDataProvider(std::string const &dbname)
:
    _dbname(dbname),
    _db(0)
{
    if (SQLITE_OK != sqlite3_open_v2(_dbname.c_str(), &_db, SQLITE_OPEN_READONLY, NULL)) {
        std::cerr << "Cannot open sqlite_db: " << sqlite3_errmsg(_db) << std::endl;
        sqlite3_close(_db);
        _db = 0;
    }
}

bool traffic::SqliteDataProvider::valid() const {
    return _db != 0;
}

traffic::SqliteDataProviderFactory::SqliteDataProviderFactory(std::string const &dbname)
:
    _dbname(dbname)
{ }


traffic::DataProvider::ptr_t traffic::SqliteDataProviderFactory::instance()
{
    if (!(_instance && !_instance->valid())) {
        _instance.reset(new SqliteDataProvider(_dbname));
    }
    return _instance;
}

thread_local std::shared_ptr<traffic::SqliteDataProvider> traffic::SqliteDataProviderFactory::_instance;
