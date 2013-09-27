/*
 * tester.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: David Tagatac
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

#define TEST_USERFILE "david student\njill student ta\n"
#define TEST_USER1 "david"
#define TEST_USER2 "jill"
#define TEST_GROUP1 "student"
#define TEST_GROUP2 "ta"
#define TEST_FILE "newfile"
#define TEST_CONTENTS "This is test file content.\n"
#define TEST_ACL "david.student	rwxpv\njill.ta	rxv\n"

TEST(UserfileTestTest, Belongs)
{
	EXPECT_TRUE(userfileTest(TEST_USER1, TEST_GROUP1));
}
TEST(UserfileTestTest, DoesNotBelong)
{
	EXPECT_FALSE(userfileTest(TEST_USER1, TEST_GROUP2));
}

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
		object =  new Object(TEST_USER1, TEST_FILE);
		objpath = fs::path(DATA_DIR);
		objpath /= TEST_USER1;
		objpath /= TEST_FILE;
		aclpath = fs::path(DATA_DIR);
		aclpath /= ACL_MANAGER;
		aclpath /= (string) TEST_USER1 + OWNER_DELIMITER + TEST_FILE;
	}

	Object *object;
	fs::path objpath, aclpath;
	string contents;
	istreambuf_iterator<char> eos;
};
TEST_F(ObjectTest, Constructor)
{
	ASSERT_TRUE(fs::exists(aclpath));
	ifstream aclstream(aclpath.c_str());
	contents = string(istreambuf_iterator<char>(aclstream), eos);
	EXPECT_EQ((string) TEST_USER1 + ".*" + GROUP_DELIMITER + DEFAULT_PERMISSIONS
			  + '\n', contents);
}
TEST_F(ObjectTest, PreExists)
{
	EXPECT_FALSE(object->exists());
}
TEST_F(ObjectTest, Put)
{
	object->put(TEST_CONTENTS);
	ifstream objectstream(objpath.c_str());
	contents = string(istreambuf_iterator<char>(objectstream), eos);
	EXPECT_EQ(TEST_CONTENTS, contents);
}
TEST_F(ObjectTest, Exists)
{
	EXPECT_TRUE(object->exists());
}
TEST_F(ObjectTest, Get)
{
	object->get(contents);
	EXPECT_EQ(TEST_CONTENTS, contents);
}
TEST_F(ObjectTest, SetACL)
{
	object->setACL(TEST_ACL);
	ifstream aclstream(aclpath.c_str());
	contents = string(istreambuf_iterator<char>(aclstream), eos);
	EXPECT_EQ(TEST_ACL, contents);
}
TEST_F(ObjectTest, GetACL)
{
	object->getACL(contents);
	EXPECT_EQ(TEST_ACL, contents);
}
TEST_F(ObjectTest, testACL)
{
	EXPECT_TRUE(object->testACL(TEST_USER1, TEST_GROUP1, 'r'));
	EXPECT_FALSE(object->testACL(TEST_USER2, TEST_GROUP1, 'w'));
}

int main(int argc, char *argv[])
{
	// Write the test userfile
	fs::path userfilepath(USERFILE);
	string contents = TEST_USERFILE;
	ofstream userfilestream(userfilepath.c_str());
	userfilestream.write(contents.c_str(), sizeof(char) * contents.size());
	userfilestream.close();

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
