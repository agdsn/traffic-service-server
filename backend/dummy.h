#ifndef __BACKEND__DUMMY_H__
#define __BACKEND__DUMMY_H__

#include "backend.h"


namespace traffic {

class DummyProvider : public DataProvider
{
public:
    ReplyMessage fetch_summary(SummaryRequest const &);

    ReplyMessage fetch_statistic(StatisticRequest const &);
};

class DummyProviderFactory : public DataProviderFactory
{
    DataProvider::ptr_t _instance;

public:
    DataProvider::ptr_t instance();
};
}

#endif
