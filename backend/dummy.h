#ifndef __BACKEND__DUMMY_H__
#define __BACKEND__DUMMY_H__

#include "backend.h"


namespace traffic {

class DummyProvider : public DataProvider
{
public:
    virtual ReplyMessage::ptr_t &&fetch_summary(SummaryRequest const &) {
        //return std::move(ReplyMessage::ptr_t());
        return std::move(ReplyMessage::ptr_t(new ErrorReply(0, "PENG")));
    }

    virtual ReplyMessage::ptr_t &&fetch_statistic(StatisticRequest const &) {
        //return std::move(ReplyMessage::ptr_t());
        return std::move(ReplyMessage::ptr_t(new ErrorReply(1, "PLONG")));
    }
};

class DummyProviderFactory : public DataProviderFactory
{
    DataProvider::ptr_t _instance;

public:
    DataProvider::ptr_t instance() {
        if (!_instance)
            _instance.reset(new DummyProvider);
        return _instance;
    }

};
}

#endif
