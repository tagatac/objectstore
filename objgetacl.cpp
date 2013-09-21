/*
 * objgetacl.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: tag
 */

#include "common.h"
#include "Object.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	string username, groupname, objname, owner, filename;
	string desc = "objgetacl - Writes an object's ACL to stdout.";
	defaultCmdLine(username, groupname, objname, desc, argc, argv);

	parseObjname(username, objname, owner, filename);
	Object thisObject(owner, filename);
	if (thisObject.testACL(username, groupname, 'r'))
	{
		string contents;
		int retval;
		if (!(retval = thisObject.getACL(contents)))
			cout << contents << endl;
		return retval;
	}
	else
	{
		cerr << "User '" << username
			 << "' does not have permission to get the ACL for object '"
			 << objname << "'." << endl;
		return 1;
	}
}
