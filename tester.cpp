/*
 * tester.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: tag
 */

#include "gtest/gtest.h"
#include "common.h"
#include "Object.h"
#include <string>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

#define TEST_USER1 "david"
#define TEST_USER2 "jill"
#define TEST_GROUP "students"
#define TEST_FILE "newfile"
#define TEST_CONTENTS "This is test file content.\n"

class ParseObjnameTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		username = TEST_USER1;
	}

	string username, objname, owner, filename;
};
TEST_F(ParseObjnameTest, SimpleFilename)
{
	objname = TEST_FILE;
	parseObjname(username, objname, owner, filename);
	EXPECT_EQ(TEST_FILE, filename);
	EXPECT_EQ(TEST_USER1, owner);
}
TEST_F(ParseObjnameTest, CompoundFilename)
{
	objname = (string) TEST_USER2 + OWNER_DELIMITER + TEST_FILE;
	parseObjname(username, objname, owner, filename);
	EXPECT_EQ(TEST_FILE, filename);
	EXPECT_EQ(TEST_USER2, owner);
}

class ObjectTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		Object object(TEST_USER1, TEST_FILE);
		objpath = fs::path(DATA_DIR);
		objpath /= TEST_USER1;
		objpath /= TEST_FILE;
		cout << "checkpoint 4 for " << objpath << endl;
		aclpath = fs::path(DATA_DIR);
		aclpath /= ACL_MANAGER;
		aclpath /= (string) TEST_USER1 + OWNER_DELIMITER + TEST_FILE;
		cout << "checkpoint 5 for " << objpath << endl;
	}

	Object *object;
	fs::path objpath, aclpath;
	string contents;
	istreambuf_iterator<char> eos;
};
/*TEST_F(ObjectTest, Constructor)
{
	ASSERT_TRUE(fs::exists(aclpath));
	ifstream aclstream(aclpath.c_str());
	// put a stream into a string (from http://stackoverflow.com/questions/3203452/how-to-read-entire-stream-into-a-stdstring)
	contents = string(istreambuf_iterator<char>(aclstream), eos);
	EXPECT_EQ((string) TEST_USER1 + ".*\t" + DEFAULT_PERMISSIONS + '\n',
			  contents);
}*/
TEST_F(ObjectTest, PreExists)
{
	cout << "checkpoint 6 for " << objpath << endl;
	cout << (long int) object << endl;
	EXPECT_FALSE(object->exists());
	cout << "checkpoint 7 for " << objpath << endl;
}
/*TEST_F(ObjectTest, Put)
{
	object->put(TEST_CONTENTS);
	EXPECT_EQ(TEST_CONTENTS, contents);
}
TEST_F(ObjectTest, PostExists)
{
	EXPECT_TRUE(object->exists());
}
TEST_F(ObjectTest, Get)
{
	object->get(contents);
	EXPECT_EQ(TEST_CONTENTS, contents);
}
*/
int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
