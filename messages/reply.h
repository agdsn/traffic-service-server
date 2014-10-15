#ifndef __MESSAGES__REPLY_H__
#define __MESSAGES__REPLY_H__

#include <string>
#include <memory>


namespace replies {
	class Statistic;
	class Summary;
	class Error;
	class Reply;
}


namespace traffic {

using traffic_t = long long;

class ReplyMessage
{
protected:
	std::unique_ptr<replies::Reply> const _reply_msg;
public:
	virtual void serialize(std::string &) const;

	ReplyMessage();
	virtual ~ReplyMessage() { }
};


class SummaryReply : public ReplyMessage
{
private:
	replies::Summary* _summary_msg;

public:
	void add_entry(std::string const &address,
			traffic_t in, traffic_t out);

	SummaryReply();
};


class StatisticReply : public ReplyMessage
{
private:
	replies::Statistic* _statistic_msg;

public:
	StatisticReply(time_t interval, traffic_t in, traffic_t out);

	void add_slice(time_t timestamp, traffic_t in, traffic_t out);
};


class ErrorReply : public ReplyMessage
{
private:
	replies::Error* _error_msg;

public:
	ErrorReply(uint32_t code, std::string const &message);
};

}

#endif
