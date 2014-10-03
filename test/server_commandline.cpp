#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "commandline.h"


TEST(ServerCommandline, test_help) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-h"};

	ASSERT_FALSE(cmd.parse(2, argv));
}


TEST(ServerCommandline, test_bind_default) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar"};

	EXPECT_TRUE(cmd.parse(1, argv));

	std::vector<std::string> addr(cmd.addresses());
	EXPECT_EQ(addr.size(), 1U);
	EXPECT_EQ(addr[0], "*:3444");
}


TEST(ServerCommandline, test_bind_single) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-b", "abcde"};

	EXPECT_TRUE(cmd.parse(3, argv));

	std::vector<std::string> addr(cmd.addresses());
	EXPECT_EQ(addr.size(), 1U);
	EXPECT_EQ(addr[0], "abcde");
}


TEST(ServerCommandline, test_bind_multi) {
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


TEST(ServerCommandline, test_bind_missing) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-b"};

	ASSERT_FALSE(cmd.parse(2, argv));
}
