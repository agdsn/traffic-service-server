#include "dummy.h"


#include <utility>


traffic::ReplyMessage::ptr_t &&traffic::DummyProvider::fetch_summary(SummaryRequest const &)
{
    return std::move(ReplyMessage::ptr_t(new ErrorReply(0, "PENG")));
}

traffic::ReplyMessage::ptr_t &&traffic::DummyProvider::fetch_statistic(StatisticRequest const &)
{
    return std::move(ReplyMessage::ptr_t(new ErrorReply(1, "PLONG")));
}

traffic::DataProvider::ptr_t traffic::DummyProviderFactory::instance()
{
    if (!_instance)
        _instance.reset(new DummyProvider);
    return _instance;
}