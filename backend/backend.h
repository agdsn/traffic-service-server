#ifndef __BACKEND__DATA_FETCH_H__
#define __BACKEND__DATA_FETCH_H__

#include "reply_message.h"
#include "request_message.h"

namespace traffic {

/**
 * \brief This is the main backend data providing interface.
 *
 * This interface has to be implemented for any data backend. It provides
 * methods to fetch the data from the database and compose the reply.
 *
 * Its creation is controlled by the DataProviderFactory, thick is asked for
 * an instance() for every request. Its up to the factory to have one instance,
 * one per worker(thread) or one per request.
 */
class DataProvider {
public:
	/**
	 * \brief Define a shared_ptr as pointer for the lazy programmer.
	 */
	using ptr_t = std::shared_ptr<DataProvider>;

	/**
	 * \brief Process a summary request.
	 *
	 * Summary requests get one or many addresses and a timerange and
	 * compose a reply with the sum of input and output bytes for each
	 * address. The number of addresses can be limited in the
	 * implementation by returning an traffic::ErrorReply message that will
	 * be delivered to the client. This can also be done for other errors.
	 *
	 * The traffic::ReplyMessage cannot be copied on return and must be
	 * moved!
	 *
	 * \param request The request data for this request
	 * \return Either a traffic::SummaryReply or a traffic::ErrorReply.
	 */
	virtual ReplyMessage fetch_summary(SummaryRequest const &request) = 0;

	/**
	 * \brief Process a statistic request.
	 *
	 * Statistic requests get one address, an interval and a timerange and
	 * compose a reply with imout and output sums at the requested interval
	 * granularity within the requested timerange. On error a
	 * traffic::ErrorReply message can be returned and will be delivered to
	 * the client.
	 *
	 * The traffic::ReplyMessage cannot be copied on return and must be
	 * moved!
	 *
	 * \param request The request data for this request
	 * \return Either a traffic::StatisticReply or a traffic::ErrorReply.
	 */
	virtual ReplyMessage fetch_statistic(StatisticRequest const &request) = 0;
};


/**
 * \brief This is a factory for creating DataProviders.
 *
 * Each worker will have a reference to it and use it to get a provider for
 * each request. It is up to the factory which parts of the provider
 * implementation are shared between the worker(threads) and the requests.
 *
 * NOTE: There is only one instance shared across all workers. So you have to
 *       guard access to shared data either by locks or use thread-local storage
 *       types.
 */
class DataProviderFactory
{
public:
	/**
	 * \brief Create (or get) a DataProvider instance.
	 *
	 * This will be called from the workers on each request to get a
	 * DataProvider instance to work with. The Factory have to care about
	 * data races between the worker threads.
	 *
	 * The worker will release its reference of the provider at the end of
	 * the request. So its up to the factory to store the pointer to
	 * preserve the provider for another request or to drop it.
	 *
	 * \return A DataProvider to use from the workers.
	 */
	virtual DataProvider::ptr_t instance() = 0;
};

}

#endif
