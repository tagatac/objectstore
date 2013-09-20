/*
 * Object.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: tag
 */

#include "Object.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

Object::Object(string o, string f)
{
	owner = o;
	filename = f;
	objpath = fs::path(DATA_DIR);
	objpath /= owner;
	objpath /= filename;
	cout << objpath << endl;

	// Create a pointer to the ACL (but only if this Object is not an ACL)
	if (owner != ACL_MANAGER)
	{
		cout << "checkpoint 1 for " << objpath << endl;
		Object thisACL(ACL_MANAGER, owner+'+'+filename);
		ACL = &thisACL;

		// If the file doesn't yet exist, create a default ACL for it
		cout << "checkpoint 2 for " << objpath << endl;
		if (!exists() && !ACL->exists())
		{
			ACL->put(owner + ".*\t" + DEFAULT_PERMISSIONS + '\n');
			cout << "checkpoint 3 for " << objpath << endl;
		}
	}
}

bool Object::exists()
{
	cout << "exists for " << objpath << endl;
	return fs::exists(objpath);
}

int Object::put(string contents)
{
	cout << "objput for " << objpath << endl;
	string line;
	istringstream ss(contents);

	// Create the directory tree down to the user's directory
	fs::path objdir(DATA_DIR);
	objdir /= owner;
	fs::create_directories(objdir);

	// Transfer the data from the passed string to the file
	ofstream objectstream(objpath.c_str());
	cout << "objput meat for " << objpath << endl;
	objectstream.write(contents.c_str(), sizeof(char) * contents.size());
	cout << "objput post for " << objpath << endl;

	return 0;
}

int Object::get(string &contents)
{
	cout << objpath << endl;
	string line;
	ostringstream ss;

	// Open the file
	ifstream objectstream(objpath.c_str());
	if (!objectstream)
	{
		cerr << "Invalid file: " << objpath << endl;
		return 1;
	}
	// Write the contents of the file to stdout
	else
		while (getline(objectstream, line)) ss << line << endl;

	return 0;
}

bool Object::testACL(string username, string groupname, char access)
{
	if (username == ACL_MANAGER) return true;

	string permissions = ""; // give no permissions by default
	string contents, aclline;

	ACL->get(contents);
	istringstream ss(contents);
	while (getline(ss, aclline))
	{
		int cursor1 = aclline.find(USER_DELIMITER);
		string user = aclline.substr(0, cursor1);
		if (user == "*" || user == username)
		{
			int cursor2 = aclline.find(GROUP_DELIMITER);
			string group = aclline.substr(cursor1 + 1, cursor2);
			if (group == "*" || group == groupname)
			{
				permissions = aclline.substr(cursor2 + 1);
				break;
			}
		}
	}

	if (permissions.find(access) != string::npos) return true;
	else return false;
}
