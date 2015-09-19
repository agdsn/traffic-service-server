#ifndef __SERVER__COMMANDLINE_H__
#define __SERVER__COMMANDLINE_H__

#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace traffic {
namespace po = boost::program_options;

/**
 * \brief This handles all commandline parsing for the daemon.
 *
 * This uses boost::program_options to parse the commandline arguments
 * of the program and provide them in a abstracted way.
 */
class Commandline
{
public:
	enum StorageType {
		SQLITE,
		MYSQL,
		POSTGRES
	};

private:
	po::options_description _desc;
	po::variables_map _vm;

	Commandline(Commandline const &) = delete;
	Commandline& operator=(Commandline const &) = delete;

public:

	/**
	 * \brief Give the specifies bind addresses.
	 *
	 * This is a list of addresses the daemon should bind to to
	 * listen for client connections.
	 */
	std::vector<std::string> addresses() const;

	/**
	 * \brief workers to spawn.
	 *
	 * Give the mumber of workers to spawn as specified at the
	 * commandline.
	 */
	unsigned int worker() const;

	/**
	 * \brief Give the configured storage type.
	 *
	 * The storage type set per commandline option.
	 */
	StorageType storage_type() const;

	/**
	 * \brief Name of the incomming tradffic table.
	 */
	std::string table_incomming() const;

	/**
	 * \brief Name of the outgoing traffic table.
	 */
	std::string table_outgoing() const;

	/**
	 * \brief Name of the sqlite database file.
	 */
	std::string sqlite_file() const;


	std::string host() const;


	unsigned int port() const;


	std::string user() const;


	std::string password() const;


	std::string database() const;


	/**
	 * \brief Parse the commandline arguments.
	 *
	 * This parses the argv array to get all the arguments. It
	 * returns true if the parsing was successfull and the program
	 * can proceed.
	 *
	 * \param argc The argument count.
	 * \param argv The argv array.
	 * \return true if the programm can proceed.
	 */
	bool parse(int argc, char const *argv[]);

	Commandline();
	~Commandline() { }
};

}

#endif
