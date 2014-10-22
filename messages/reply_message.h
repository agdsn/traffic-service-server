#ifndef __MESSAGES__REPLY_MESSAGE_H__
#define __MESSAGES__REPLY_MESSAGE_H__

#include <string>
#include <memory>


namespace replies {
	class Statistic;
	class Summary;
	class Error;
	class Reply;
}


namespace traffic {

/**
 * \brief Type alias for traffic amounts
 */
using traffic_t = long long;

/**
 * \brief Generic base class for all reply message wrappers.
 *
 * It provides serializing withut any knowledge about the underlying protobuf
 * structures.
 */
class ReplyMessage
{
protected:
	std::unique_ptr<replies::Reply> const _reply_msg;
public:

	/**
	 * \brief Serialize the message to the wire format.
	 *
	 * \param output The output string where the message should be placed.
	 */
	void serialize(std::string &output) const;

	ReplyMessage();
	virtual ~ReplyMessage() { }
};


/**
 * \brief This wraps the reply messages for summry requests.
 */
class SummaryReply : public ReplyMessage
{
private:
	replies::Summary* _summary_msg;

public:
	/**
	 * \brief Add a summary data entry to the list of summaries.
	 *
	 * \param address The string representation of the ip address.
	 * \param in      The summarized input amount for the ip.
	 * \param out     The summarized output amount for the ip.
	 */
	void add_entry(std::string const &address,
			traffic_t in, traffic_t out);

	SummaryReply();
};


/**
 * \brief This wraps the reply message for statistic requests.
 */
class StatisticReply : public ReplyMessage
{
private:
	replies::Statistic* _statistic_msg;

public:
	/**
	 * \brief Create a reply message.
	 *
	 * \param interval The interval width of the slices.
	 * \param in       The summarized input bytes.
	 * \param out      The summarized output bytes.
	 */
	StatisticReply(time_t interval, traffic_t in, traffic_t out);

	/**
	 * \brief Add a statistic slize
	 *
	 * This represents the value within a interval.
	 *
	 * \param timestamp The timestamp of the slice.
	 * \param in        The recieved bytes within the interval.
	 * \param out       The sent bytes within the interval.
	 */
	void add_slice(time_t timestamp, traffic_t in, traffic_t out);
};


/**
 * \brief This wraps a error reply.
 *
 * The error reply is used in any case of error and contains a number to
 * identify the error type and a message which describes the error.
 */
class ErrorReply : public ReplyMessage
{
private:
	replies::Error* _error_msg;

public:
	/**
	 * \brief Create the error message.
	 *
	 * \param code    The error number to identify the error type.
	 * \param message The error message in human readable terms.
	 */
	ErrorReply(uint32_t code, std::string const &message);
};

}

#endif
