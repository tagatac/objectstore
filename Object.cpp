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
#include <boost/regex.hpp>
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
	}
}

bool Object::exists()
{
	return fs::exists(objpath);
}

int Object::put(string contents)
{
	// If the file doesn't yet exist, create a default ACL for it
	if (owner != ACL_MANAGER && !exists() && !ACL->exists())
	{
		ACL->put(owner + ".*" + GROUP_DELIMITER + DEFAULT_PERMISSIONS + '\n');
	}

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
	fs::path userfilepath(USERFILE);
	string userfileline;
	string validusers[MAX_USERS];
	int counter = 0;

	// Open the userfile.
	fs::ifstream userfilestream(userfilepath);
	if (!userfilestream)
	{
		cerr << "Invalid userfile!" << endl;
		return false;
	}
	// Extract all of the valid users from the userfile.
	while (getline(userfilestream, userfileline))
	{
		size_t cursor = userfileline.find(USERFILE_DELIMITER);
		validusers[counter] = userfileline.substr(0, cursor);
		counter++;
	}

	string aclline;
	istringstream ss(contents);
	static const boost::regex permissionsExpr("r?w?x?p?v?");
	bool aclvalidity = true;
	while (getline(ss, aclline))
	{
		// First validate the user.
		size_t cursor1 = aclline.find(USER_DELIMITER);
		if (cursor1 == string::npos)
		{
			aclvalidity = false;
			break;
		}
		string user = aclline.substr(0, cursor1);
		if (user != "*" && find(begin(validusers), end(validusers), user) == end(validusers))
		{
			aclvalidity = false;
			break;
		}
		// If the user is valid, validate the group.
		int cursor2 = aclline.find(GROUP_DELIMITER);
		if (cursor2 == string::npos)
		{
			aclvalidity = false;
			break;
		}
		string group = aclline.substr(cursor1 + 1, cursor2 - (cursor1 + 1));
		// If the user and the group match, use these permissions.
		string permissions = aclline.substr(cursor2 + 1);
		if (!regex_match(permissions, permissionsExpr))
		{
			aclvalidity = false;
			break;
		}
	}

	if (!aclvalidity)
	{
		cerr << "Invalid ACL format" << endl;
		cerr << "Valid format is one or more lines of '<user>.<group>\\t[r][w][x][p][v]\\n'" << endl;
		return 1;
	}
	else
		return ACL->put(contents);
}

int Object::getACL(string &contents)
{
	return ACL->get(contents);
}

bool Object::testACL(string username, string groupname, char access)
{
	if (!ACL->exists())
	{
		if (username == owner) return true;
		else return false;
	}

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
