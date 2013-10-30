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
#include <boost/lexical_cast.hpp>
using namespace std;

int main(int argc, char **argv)
{
	string objname, owner, filename;
	string desc = "objsetacl - Reads an ACL from stdin and sets it.";
	defaultCmdLine(objname, desc, argc, argv);

	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (!thisObject.exists())
	{
		cerr << "Error: Attempting to set an ACL for a non-existent object." << endl;
		return 1;
	}
	if (thisObject.testACL('p'))
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
		string username = boost::lexical_cast<string>(getuid());
		cerr << "You do not have permission to set the ACL for object '"
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
