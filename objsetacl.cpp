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
	if (!thisObject.exists())
	{
		cerr << "Error: Attempting to set an ACL for a non-existent object." << endl;
		return 1;
	}
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
		if (username == owner)
		{
			cout << "As the owner of this object, would you like to reset the "
				 << "ACL to the default? [y/N]" << endl;
			string response = "";
			cin >> response;
			if (response.length() > 0 && tolower(response[0]) == 'y')
				thisObject.setACL(owner + ".*" + GROUP_DELIMITER + DEFAULT_PERMISSIONS + '\n');
			cout << "ACL reset to default." << endl;
		}
		return 1;
	}
}
