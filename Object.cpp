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
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

Object::Object(string o, string f)
{
	owner = o;
	filename = f;
	if (owner != "data-acl")
	{
		Object thisACL("data-acl", owner+'+'+filename);
		ACL = &thisACL;
	}
}

int Object::put()
{
	string line;

	// Create the directory tree down to the user's directory
	fs::path objdir("data");
	cout << owner;
	objdir /= owner;
	fs::create_directories(objdir);

	// Transfer the data from stdin to the file
	fs::path objpath(objdir);
	objpath /= filename;
	ofstream objectstream(objpath.c_str());
	while (getline(cin, line)) objectstream << line << endl;

	return 0;
}

int Object::get()
{
	string line;

	// Open the file
	fs::path objpath("data");
	objpath /= owner;
	objpath /= filename;
	ifstream objectstream(objpath.c_str());
	if (!objectstream)
	{
		cerr<< "Invalid file: " << objpath << endl;
		return 1;
	}
	// Write the contents of the file to stdout
	else
		while (getline (objectstream, line)) cout << line << endl;

	return 0;
}

bool Object::testACL(string username, string groupname, char access)
{
	return true;
}
