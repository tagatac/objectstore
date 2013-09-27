/*
 * objsetacl.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: David Tagatac
 */

#include "common.h"
#include "Object.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	string username, groupname, objname, owner, filename;
	string desc = "objsetacl - Reads an ACL from stdin and sets it.";
	defaultCmdLine(username, groupname, objname, desc, argc, argv);

	parseObjname(username, objname, owner, filename);
	Object thisObject(owner, filename);
	if (userfileTest(username, groupname) && thisObject.testACL(username, groupname, 'p'))
	{
		// Prompt the user to enter data
		cout << "Please enter file content:" << endl;
		// Put the stream into a string
		string contents;
		istreambuf_iterator<char> eos;
		contents = string(istreambuf_iterator<char>(cin), eos);
		// Create the object on disk
		return thisObject.setACL(contents);
	}
	else
	{
		cerr << "User '" << username
			 << "' does not have permission to set the ACL for object '"
			 << objname << "'." << endl;
		return 1;
	}
}
