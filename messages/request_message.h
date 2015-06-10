#ifndef __MESSAGES__REQUEST_H__
#define __MESSAGES__REQUEST_H__

#include <memory>
#include <vector>
#include "common_message.h"

namespace requests {

class Statistic;
class Summary;
}


namespace traffic {


class RequestVisitor;


class RequestMessage
{
public:
	using ptr_t = std::shared_ptr<RequestMessage>;

	static ptr_t parse_message(void const *data, size_t const size);

	virtual void accept(RequestVisitor &visitor) = 0;

	virtual ~RequestMessage() { }
};


class StatisticRequest : public RequestMessage
{
public:
	enum Interval {
		DAY = 0,
		HOUR = 1,
		DETAIL = 2
	};

private:
	TimeRange const _range;
	std::string const _address;
	Interval const _interval;

public:

	virtual void accept(RequestVisitor &visitor);

	StatisticRequest(requests::Statistic const &msg);

	const TimeRange &range() const { return _range; }
	const std::string &address() const { return _address; }
	const Interval &interval() const { return _interval; }
};


class SummaryRequest : public RequestMessage
{
private:
	TimeRange const _range;
	std::vector<std::string> const _addresses;

public:
	virtual void accept(RequestVisitor &visitor);

	SummaryRequest(requests::Summary const &msg);

	const TimeRange &range() const { return _range; }
	const std::vector<std::string> &addresses() const { return _addresses; }

};


class ErrorRequest : public RequestMessage
{
public:
	virtual void accept(RequestVisitor &visitor);

	ErrorRequest() { }
};


class RequestVisitor
{
protected:
	virtual void visit(StatisticRequest const &request) = 0;
	virtual void visit(SummaryRequest const &request) = 0;
	virtual void visit(ErrorRequest const &request) = 0;

public:
	virtual ~RequestVisitor() { }

	friend class StatisticRequest;
	friend class SummaryRequest;
	friend class ErrorRequest;
};


}

#endif
