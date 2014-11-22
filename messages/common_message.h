#ifndef __MESSAGES__COMMON_MESSAGE_H__
#define __MESSAGES__COMMON_MESSAGE_H__

#include <ctime>


namespace traffic {


/**
 * \brief Wraps the TimeRange message type.
 */
class TimeRange
{
private:
	time_t const _start;
	time_t const _end;

public:
	/**
	 * \brief The start of the interval.
	 */
	time_t start() const { return _start; }

	/**
	 * \brief The end of the interval.
	 */
	time_t end() const { return _end; }

	/**
	 * \brief Tells if this is a valid interval.
	 *
	 * A valid interval is one where the start is before the
	 * end timesoamp.
	 */
	bool valid() const { return _end > _start; }

	TimeRange(time_t const start, time_t const end);
};

}

#endif
