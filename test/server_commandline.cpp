#include "gtest/gtest.h"

#include "commandline.h"

TEST(ServerCommandline, test_help) {
	traffic::Commandline cmd;
	const char* argv[] = {"foobar", "-h"};

	ASSERT_FALSE(cmd.parse(2, argv));
}
