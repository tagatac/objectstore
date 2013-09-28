/*
 * Object.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#include "Object.h"
#include "common.h"
#include <string>
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace std;
namespace fs = boost::filesystem;

Object::Object(string o, string f)
{
	owner = o;
	filename = f;
	objpath = fs::path(DATA_DIR);
	objpath /= owner;
	objpath /= filename;

	// Create a pointer to the ACL (but only if this Object is not an ACL)
	if (owner != ACL_MANAGER)
	{
		ACL = new Object(ACL_MANAGER, owner + OWNER_DELIMITER + filename);

		// If the file doesn't yet exist, create a default ACL for it
		if (!exists() && !ACL->exists())
		{
			ACL->put(owner + ".*" + GROUP_DELIMITER + DEFAULT_PERMISSIONS + '\n');
		}
	}
}

bool Object::exists()
{
	return fs::exists(objpath);
}

int Object::put(string contents)
{
	// Create the directory tree down to the user's directory
	fs::path objdir(DATA_DIR);
	objdir /= owner;
	fs::create_directories(objdir);

	// Transfer the data from the passed string to the file
	fs::ofstream objectstream(objpath);
	objectstream.write(contents.c_str(), sizeof(char) * contents.size());

	return 0;
}

int Object::get(string &contents)
{
	// Open the file
	fs::ifstream objectstream(objpath);
	if (!objectstream)
	{
		cerr << "Invalid file: " << objpath << endl;
		return 1;
	}
	// Write the contents of the file to stdout
	istreambuf_iterator<char> eos;
	// Put the stream into a string (from http://stackoverflow.com/questions/3203452/how-to-read-entire-stream-into-a-stdstring)
	contents = string(istreambuf_iterator<char>(objectstream), eos);

	return 0;
}

int Object::setACL(string contents)
{
	return ACL->put(contents);
}

int Object::getACL(string &contents)
{
	return ACL->get(contents);
}

bool Object::testACL(string username, string groupname, char access)
{
	if (username == ACL_MANAGER) return true;

	string permissions = ""; // Give no permissions by default.
	string contents, aclline;

	ACL->get(contents);
	istringstream ss(contents);
	// Parse each line of the ACL until a match is found.
	while (getline(ss, aclline))
	{
		// First match the user.
		int cursor1 = aclline.find(USER_DELIMITER);
		string user = aclline.substr(0, cursor1);
		if (user == "*" || user == username)
		{
			// If the user matches, match the group.
			int cursor2 = aclline.find(GROUP_DELIMITER);
			string group = aclline.substr(cursor1 + 1, cursor2 - (cursor1 + 1));
			if (group == "*" || group == groupname)
			{
				// If the user and the group match, use these permissions.
				permissions = aclline.substr(cursor2 + 1);
				break;
			}
		}
	}

	// If the desired access character is in the permissions string, return true
	return (permissions.find(access) != string::npos);
}
