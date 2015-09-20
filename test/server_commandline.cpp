#include <vector>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>

#include "gtest/gtest.h"

#include "commandline.h"

#define REQUIRED_TABLES "-I", "a", "-O", "b"
#define REQUIRED_STORAGE "-s", "sqlite", "--sqlite_file", "abc"
#define REQUIRED REQUIRED_STORAGE, REQUIRED_TABLES


class ServerCommandline : public ::testing::Test
{
private:
	std::stringstream _std_out_stream;
	std::stringstream _std_err_stream;
	std::streambuf *_orig_out;
	std::streambuf *_orig_err;

public:
	ServerCommandline()
	:
		_std_out_stream(),
		_std_err_stream(),
		_orig_out(std::cout.rdbuf()),
		_orig_err(std::cerr.rdbuf())
	{
		std::cout.rdbuf(_std_out_stream.rdbuf());
		std::cerr.rdbuf(_std_err_stream.rdbuf());
	}

	~ServerCommandline()
	{
		std::cout.rdbuf(_orig_out);
		std::cerr.rdbuf(_orig_err);
	}

	std::string output() const
	{
		return _std_out_stream.str();
	}

	std::string error() const
	{
		return _std_err_stream.str();
	}
};


// Test if -h prints help and exits
TEST_F(ServerCommandline, test_help) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-h"};

	EXPECT_FALSE(cmd.parse(2, argv));

	EXPECT_NE(output().find("Print help message."), std::string::npos);
}


// Test if default bind works
TEST_F(ServerCommandline, test_bind_default) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", REQUIRED};

	EXPECT_TRUE(cmd.parse(1 + 4 + 4, argv));

	std::vector<std::string> addr(cmd.addresses());
	EXPECT_EQ(addr.size(), 1U);
	EXPECT_EQ(addr[0], "*:3444");
}


// Test if we can bind a single address
TEST_F(ServerCommandline, test_bind_single) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-b", "abcde", REQUIRED};

	EXPECT_TRUE(cmd.parse(3 + 4 + 4, argv));

	std::vector<std::string> addr(cmd.addresses());
	EXPECT_EQ(addr.size(), 1U);
	EXPECT_EQ(addr[0], "abcde");
}


// Test if we can bind multiple addresses
TEST_F(ServerCommandline, test_bind_multi) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar",
		"-b", "abcde",
		"--bind", "1234",
		"-b", "12ab",
		REQUIRED};

	EXPECT_TRUE(cmd.parse(7 + 4 + 4, argv));

	std::vector<std::string> addr(cmd.addresses());
	EXPECT_EQ(addr.size(), 3U);
	EXPECT_EQ(addr[0], "abcde");
	EXPECT_EQ(addr[1], "1234");
	EXPECT_EQ(addr[2], "12ab");
}


// Test if we get an error if bind arg is omitted
TEST_F(ServerCommandline, test_bind_missing) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", REQUIRED, "-b"};

	EXPECT_FALSE(cmd.parse(2 + 4 + 4, argv));
	EXPECT_NE(error().find("the required argument for "
			       "option '--bind' is missing"),
		  std::string::npos);
}


// Test missing storage type
TEST_F(ServerCommandline, test_missing_storage_type) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", REQUIRED_TABLES};

	EXPECT_FALSE(cmd.parse(1 + 4, argv));
	EXPECT_NE(error().find("the option '--storage' "
			       "is required but missing"),
		  std::string::npos);
}


// Test for sqlite storage type
TEST_F(ServerCommandline, test_storage_sqlite) {
	{
		traffic::Commandline cmd;
		const char* argv[] = {"foobar", REQUIRED_TABLES,
				      "-s", "sqlite", "--sqlite_file", "ab"};

		EXPECT_TRUE(cmd.parse(5 + 4, argv));
		EXPECT_EQ(cmd.storage_type(), traffic::Commandline::SQLITE);
		EXPECT_EQ(cmd.sqlite_file(), "ab");
	}
	{
		traffic::Commandline cmd;
		const char* argv[] = {"foobar", REQUIRED_TABLES,
				      "-s", "sqlite"};

		EXPECT_FALSE(cmd.parse(3 + 4, argv));
		EXPECT_NE(error().find("option --sqlite_file missing for "
				       "storage_type sqlite!"),
			  std::string::npos);
	}
}


// Test for mysql storage type
TEST_F(ServerCommandline, test_storage_mysql) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-s", "mysql",
		"--host", "localhost",
		"--user", "abc",
		"--password", "cde",
		"--database", "foo",
		REQUIRED_TABLES};

	EXPECT_TRUE(cmd.parse(3 + 8 + 4, argv));

	EXPECT_EQ(cmd.storage_type(), traffic::Commandline::MYSQL);
}


// Test for postgres storage type
TEST_F(ServerCommandline, test_storage_postgres) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-s", "postgres", REQUIRED_TABLES};

	EXPECT_TRUE(cmd.parse(3 + 4, argv));

	EXPECT_EQ(cmd.storage_type(), traffic::Commandline::POSTGRES);
}


// Test for invalid storage type
TEST_F(ServerCommandline, test_storage_invalid) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-s", "peng", REQUIRED_TABLES};

	EXPECT_FALSE(cmd.parse(3 + 4, argv));

	EXPECT_NE(error().find("the argument for option "
			       "'--storage' is invalid"),
		  std::string::npos);
}


// Test for sqlite storage type
TEST_F(ServerCommandline, test_worker_default) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", REQUIRED};

	EXPECT_TRUE(cmd.parse(1 + 4 + 4, argv));

	EXPECT_EQ(cmd.worker(), 2U);
}


// Test for sqlite storage type
TEST_F(ServerCommandline, test_worker_count) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-w", "5", REQUIRED};

	EXPECT_TRUE(cmd.parse(3 + 4 + 4, argv));

	EXPECT_EQ(cmd.worker(), 5U);
}


// Test output table missing
TEST_F(ServerCommandline, test_tables_out_missing) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", REQUIRED_STORAGE, "-I", "a"};

	EXPECT_FALSE(cmd.parse(3 + 4, argv));
	EXPECT_NE(error().find("the option '--table_out' is "
				"required but missing"),
			std::string::npos);
}


// Test input table missing
TEST_F(ServerCommandline, test_tables_in_missing) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", REQUIRED_STORAGE, "-O", "a"};

	EXPECT_FALSE(cmd.parse(3 + 4, argv));
	EXPECT_NE(error().find("the option '--table_in' is "
				"required but missing"),
			std::string::npos);
}


// Test tables
TEST_F(ServerCommandline, test_tables) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", REQUIRED_STORAGE,
			      "-O", "a", "-I", "b"};

	EXPECT_TRUE(cmd.parse(5 + 4, argv));
	EXPECT_EQ(cmd.table_outgoing(), "a");
	EXPECT_EQ(cmd.table_incomming(), "b");
}
