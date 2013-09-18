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

#define ACL_MANAGER "data-acl"
#define DATA_DIR "data"
#define DEFAULT_PERMISSIONS "rwxpv"
#define USER_DELIMITER '.'
#define GROUP_DELIMITER '\t'

Object::Object(string o, string f)
{
	owner = o;
	filename = f;
	objpath = DATA_DIR;
	objpath /= owner;
	objpath /= filename;

	// Create a pointer to the ACL (but only if this Object is not an ACL)
	if (owner != ACL_MANAGER)
	{
		Object thisACL(ACL_MANAGER, owner+'+'+filename);
		ACL = &thisACL;

		// If the file doesn't yet exist, create a default ACL for it
		if (!exists())
		{
			istringstream ss(owner + ".*" + DEFAULT_PERMISSIONS);
			ACL->put(ss);
		}
	}
}

bool Object::exists()
{
	return fs::exists(objpath);
}

int Object::put(istream &thisIStream)
{
	string line;

	// Create the directory tree down to the user's directory
	fs::path objdir(DATA_DIR);
	objdir /= owner;
	fs::create_directories(objdir);

	// Transfer the data from stdin to the file
	ofstream objectstream(objpath.c_str());
	while (getline(thisIStream, line)) objectstream << line << endl;

	return 0;
}

int Object::get(ostream &thisOStream)
{
	string line;

	// Open the file
	ifstream objectstream(objpath.c_str());
	if (!objectstream)
	{
		cerr<< "Invalid file: " << objpath << endl;
		return 1;
	}
	// Write the contents of the file to stdout
	else
		while (getline(objectstream, line)) thisOStream << line << endl;

	return 0;
}

bool Object::testACL(string username, string groupname, char access)
{
	if (username == ACL_MANAGER) return true;

	string permissions = ""; // give no permissions by default
	string aclline;
	fstream thisFStream;

	ACL->get(thisFStream);
	while (getline(thisFStream, aclline))
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
