#ifndef __MESSAGES__COMMON_MESSAGE_H__
#define __MESSAGES__COMMON_MESSAGE_H__

#include <ctime>


namespace traffic {

class TimeRange
{
private:
	time_t const _start;
	time_t const _end;

public:
	time_t start() const { return _start; }
	time_t end() const { return _end; }
	bool valid() const { return _end > _start; }

	TimeRange(time_t const start, time_t const end);
};

}

#endif
