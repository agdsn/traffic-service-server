#include <vector>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>

#include "gtest/gtest.h"

#include "commandline.h"


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
	const char* argv[] = {"foobar"};

	EXPECT_TRUE(cmd.parse(1, argv));

	std::vector<std::string> addr(cmd.addresses());
	EXPECT_EQ(addr.size(), 1U);
	EXPECT_EQ(addr[0], "*:3444");
}


// Test if we can bind a single address
TEST_F(ServerCommandline, test_bind_single) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-b", "abcde"};

	EXPECT_TRUE(cmd.parse(3, argv));

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
		"-b", "12ab"};

	EXPECT_TRUE(cmd.parse(7, argv));

	std::vector<std::string> addr(cmd.addresses());
	EXPECT_EQ(addr.size(), 3U);
	EXPECT_EQ(addr[0], "abcde");
	EXPECT_EQ(addr[1], "1234");
	EXPECT_EQ(addr[2], "12ab");
}


// Test if we get an error if bind arg is omitted
TEST_F(ServerCommandline, test_bind_missing) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-b"};

	EXPECT_FALSE(cmd.parse(2, argv));
	EXPECT_NE(error().find("the required argument for "
			       "option '--bind' is missing"),
		  std::string::npos);
}
