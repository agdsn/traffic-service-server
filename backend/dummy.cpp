#include "dummy.h"


#include <utility>


traffic::ReplyMessage traffic::DummyProvider::fetch_summary(SummaryRequest const &)
{
    return ErrorReply(0, "PENG");
}

traffic::ReplyMessage traffic::DummyProvider::fetch_statistic(StatisticRequest const &)
{
    return ErrorReply(1, "PLONG");
}

traffic::DataProvider::ptr_t traffic::DummyProviderFactory::instance()
{
    if (!_instance)
        _instance.reset(new DummyProvider);
    return _instance;
}