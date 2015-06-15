#include "sqlite.h"

#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <boost/assert.hpp>

#define MAX_PARAMS 800UL


class PreparedStatement {

    sqlite3 *_db;
    sqlite3_stmt *_statement;
    int _args;
    int _next_arg;

    int _get_next_arg()
    {
        BOOST_ASSERT(_next_arg < _args);
        return ++_next_arg;
    }

public:

    enum Status {
        OK,
        FINISH,
        ERROR
    };

    void bind(std::string const &param)
    {
        sqlite3_bind_text(_statement,
                          _get_next_arg(),
                          param.c_str(),
                          -1, SQLITE_TRANSIENT);
    }

    void bind(time_t param)
    {
        sqlite3_bind_int64(_statement,
                           _get_next_arg(),
                           param);
    }

    Status step()
    {
        int state(sqlite3_step(_statement));
        if (state == SQLITE_ROW)
            return OK;
        if (state == SQLITE_DONE)
            return FINISH;
        if (state == SQLITE_ERROR)
            std::cerr << "Error running query: " << sqlite3_errmsg(_db);
        return ERROR;
    }

    std::string get_string(unsigned id)
    {
        return std::string(reinterpret_cast<const char *>(sqlite3_column_text(_statement, id)));
    }

    long long int get_long(unsigned id)
    {
        return sqlite3_column_int64(_statement, id);
    }

    void reset()
    {
	    sqlite3_clear_bindings(_statement);
            sqlite3_reset(_statement);
    }

    PreparedStatement(sqlite3 *db, std::string const &query)
    :
        _db(db),
        _statement(0),
        _args(0),
        _next_arg(0)
    {
        if (SQLITE_OK != sqlite3_prepare_v2(db, query.c_str(), -1, &_statement, NULL))
        {
            std::cerr << "Cannot prepare Statement: " << sqlite3_errmsg(db) << std::endl;
            std::cerr << query << std::endl;
            BOOST_ASSERT(false && "Statement preparation failed");
        }
        _args = sqlite3_bind_parameter_count(_statement);
    }

    ~PreparedStatement()
    {
        if (_statement) {
            sqlite3_reset(_statement);
            sqlite3_finalize(_statement);
        }
    }
};


std::string traffic::SqliteDataProvider::_summary_query(size_t hostcount) {
    std::stringstream placeholders;
    for (size_t i = 0; i < hostcount; ++i) {
        if (0 != i)
            placeholders << ", ";
        placeholders << "?";
    }

    std::stringstream query;
    query << "SELECT ip, SUM(input) AS input, SUM(output) AS output FROM ("
    << "  SELECT i.ip_dst AS ip, SUM(i.bytes) AS input, 0 AS output "
    << "    FROM " << _inbound_table << " AS i "
    << "    WHERE stamp_inserted <= ? and stamp_inserted > ?"
    << "    GROUP BY i.ip_dst"
    << "  UNION "
    << "  SELECT o.ip_src AS ip, 0 AS input, SUM(o.bytes) AS output "
    << "    FROM " << _outbound_table << " AS o "
    << "    WHERE stamp_inserted <= ? and stamp_inserted > ?"
    << "    GROUP BY o.ip_src"
    << ") WHERE ip in (" << placeholders.str() << ") GROUP BY ip";

    return query.str();
}


traffic::ReplyMessage traffic::SqliteDataProvider::fetch_summary(const traffic::SummaryRequest &request)
{
    if (! valid())
        return ErrorReply(10, "No valid database connection");
    (void)request;

    size_t address_count(request.addresses().size());
    size_t processed(0);
    size_t chunk_before(0);

    SummaryReply reply;

    std::unique_ptr<PreparedStatement> query;

    while(processed < address_count) {
        size_t chunk(std::min(MAX_PARAMS, address_count - processed));

        if (!query || chunk_before != chunk) {
	    query.reset(new PreparedStatement(_db, _summary_query(chunk)));
        } else {
            query->reset();
	}

        query->bind(request.range().end());
        query->bind(request.range().start());
        query->bind(request.range().end());
        query->bind(request.range().start());

        for (auto addr = request.addresses().begin() + processed;
                addr != request.addresses().begin() + processed + chunk;
                ++addr) {
            query->bind(*addr);
        }

        PreparedStatement::Status result;
        while ((result = query->step()) == PreparedStatement::Status::OK) {
            reply.add_entry(query->get_string(1),
                            query->get_long(2),
                            query->get_long(3));
        }

        if (result == PreparedStatement::Status::ERROR) {
            return ErrorReply(11, "Error while fetch from database");
        }

        processed += chunk;
    }

    return std::move(reply);
}


traffic::ReplyMessage traffic::SqliteDataProvider::fetch_statistic(const traffic::StatisticRequest &request)
{
    if (! valid())
        return ErrorReply(10, "No valid database connection");
    (void)request;
    return traffic::ReplyMessage();
}


traffic::SqliteDataProvider::SqliteDataProvider(std::string const &dbname,
                                                std::string const &inbound_table,
                                                std::string const &outbound_table)
:
    _dbname(dbname),
    _db(0),
    _inbound_table(inbound_table),
    _outbound_table(outbound_table)
{
    if (SQLITE_OK != sqlite3_open_v2(_dbname.c_str(), &_db, SQLITE_OPEN_READONLY, NULL)) {
        std::cerr << "Cannot open sqlite_db: " << sqlite3_errmsg(_db) << std::endl;
        sqlite3_close(_db);
        _db = 0;
    }
}


bool traffic::SqliteDataProvider::valid() const
{
    return _db != 0;
}


traffic::SqliteDataProviderFactory::SqliteDataProviderFactory(std::string const &dbname,
                                                              std::string const &inbound_table,
                                                              std::string const &outbound_table)
:
    _dbname(dbname),
    _inbound_table(inbound_table),
    _outbound_table(outbound_table)
{ }


traffic::DataProvider::ptr_t traffic::SqliteDataProviderFactory::instance()
{
    if (!(_instance && !_instance->valid())) {
        _instance.reset(new SqliteDataProvider(_dbname, _inbound_table, _outbound_table));
    }
    return _instance;
}

thread_local std::shared_ptr<traffic::SqliteDataProvider> traffic::SqliteDataProviderFactory::_instance;
