#ifndef __BACKEND__DATA_FETCH_H__
#define __BACKEND__DATA_FETCH_H__

#include "reply_message.h"
#include "request_message.h"

namespace traffic {

class DataProvider {
public:
    virtual ReplyMessage::ptr_t&& fetch_summary(SummaryRequest const &request) = 0;
	virtual ReplyMessage::ptr_t&& fetch_statistic(StatisticRequest const &request) = 0;
};

}
#endif
