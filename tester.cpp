/*
 * tester.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: tag
 */

#include "gtest/gtest.h"
#include "common.h"
#include <string>
using namespace std;

TEST(parseObjnameTest, filenameOnly)
{
	string username="david", objname="newfile", owner, filename;
	parseObjname(username, objname, owner, filename);
	EXPECT_EQ("newfile", filename);
	EXPECT_EQ("david", owner);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
