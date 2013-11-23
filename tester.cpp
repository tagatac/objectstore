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
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <openssl/md5.h>
using namespace std;
namespace fs = boost::filesystem;

#define TEST_USER1 "user"
#define TEST_USER2 "tinyvm"
#define TEST_USER3 "sidedoor"
#define TEST_GROUP1 "user"
#define TEST_GROUP2 "tinyvm"
#define TEST_FILE "newfile"
#define TEST_FILE1 "tinyvm+newfile1"
#define TEST_CONTENTS "This is test file content.\n"
#define TEST_ACL "user.user\trxpv\ntinyvm.tinyvm\trxv\n"
#define BAD_ACL1 "angela.*\trw\n"
#define BAD_ACL2 "user.user rwxpv\n"
#define TEST_FILE_HEX "6e657766696c65"
#define TEST_KEY reinterpret_cast<unsigned char *>(const_cast<char *>("912ec803b2ce49e4a541068d495ab570"))
#define TEST_CONTENTS_ENC "f8723ff414de93c19b814592c8c4cf0e56f1ead913a130f495edf7d7d810ab3e"
#define TEST_PASSPHRASE "asdf"

class ParseObjnameTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		username = boost::lexical_cast<string>(getuid());
	}

	string username, objname, owner, filename;
};
TEST_F(ParseObjnameTest, SimpleFilename)
{
	objname = TEST_FILE;
	parseObjname(objname, owner, filename);
	EXPECT_EQ(TEST_FILE, filename);
	EXPECT_EQ(username, owner);
}
TEST_F(ParseObjnameTest, CompoundFilename)
{
	objname = (string) TEST_USER2 + OWNER_DELIMITER + TEST_FILE;
	parseObjname(objname, owner, filename);
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
	EXPECT_FALSE(fs::exists(aclpath));
}
TEST_F(ObjectTest, PreExists)
{
	EXPECT_FALSE(object->exists());
}
TEST_F(ObjectTest, Put)
{
	object->put(TEST_CONTENTS);
	uid_t ruid, euid, suid;
	getresuid(&ruid, &euid, &suid);
	setuid(suid);
	fs::ifstream objectstream(objpath);
	contents = string(istreambuf_iterator<char>(objectstream), eos);
	setuid(ruid);
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
	uid_t ruid, euid, suid;
	getresuid(&ruid, &euid, &suid);
	setuid(suid);
	fs::ifstream aclstream(aclpath);
	contents = string(istreambuf_iterator<char>(aclstream), eos);
	EXPECT_EQ(TEST_ACL, contents);
	object->setACL(BAD_ACL1);
	setuid(suid);
	fs::ifstream badaclstream1(aclpath);
	contents = string(istreambuf_iterator<char>(badaclstream1), eos);
	EXPECT_EQ(TEST_ACL, contents);
	object->setACL(BAD_ACL2);
	setuid(suid);
	fs::ifstream badaclstream2(aclpath);
	contents = string(istreambuf_iterator<char>(badaclstream2), eos);
	setuid(ruid);
	EXPECT_EQ(TEST_ACL, contents);
}
TEST_F(ObjectTest, GetACL)
{
	object->setACL(TEST_ACL);
	object->getACL(contents);
	EXPECT_EQ(TEST_ACL, contents);
}
TEST_F(ObjectTest, TestACL)
{
	EXPECT_TRUE(object->testACL('r'));
	EXPECT_FALSE(object->testACL('w'));
}

TEST(ACLTest, NoObject)
{
	new Object(TEST_USER2, TEST_FILE);
	fs::path aclpath = fs::path(DATA_DIR);
	aclpath /= ACL_MANAGER;
	aclpath /= (string) TEST_USER2 + OWNER_DELIMITER + TEST_FILE;
	uid_t ruid, euid, suid;
	getresuid(&ruid, &euid, &suid);
	setuid(suid);
	EXPECT_FALSE(fs::exists(aclpath));
	setuid(ruid);
}

class CryptoTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		o = new Object("test", "test");
		MD5(reinterpret_cast<unsigned char *>(const_cast<char *>(TEST_PASSPHRASE)),
				  strlen(TEST_PASSPHRASE), key);
	}

	Object *o;
	unsigned char key[AESBLOCK];
};
TEST_F(CryptoTest, Hexify)
{
	EXPECT_EQ(TEST_FILE_HEX, hexify(reinterpret_cast<unsigned char *>(const_cast<char *>(TEST_FILE))));
	unsigned char dehexed_filename[strlen(TEST_FILE)];
	dehexify(TEST_FILE_HEX, dehexed_filename);
	EXPECT_EQ(TEST_FILE, (string) const_cast<const char *>(reinterpret_cast<char *>(dehexed_filename)));
}
TEST_F(CryptoTest, Encrypt)
{
	string encryptedContents = encrypt(TEST_CONTENTS, key, o->metadata);
	unsigned char contents[strlen(TEST_CONTENTS)];
	EXPECT_EQ(TEST_CONTENTS, decrypt(encryptedContents, o));
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
