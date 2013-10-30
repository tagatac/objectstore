/*
 * objgetacl.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: David Tagatac
 */

#include "common.h"
#include "Object.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	string objname, owner, filename;
	string desc = "objgetacl - Writes an object's ACL to stdout.";
	defaultCmdLine(objname, desc, argc, argv);

	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (thisObject.testACL('v'))
	{
		string contents;
		int retval;
		if (!(retval = thisObject.getACL(contents)))
			cout << contents << endl;
		return retval;
	}
	else
	{
		cerr << "You do not have permission to get the ACL for object '"
			 << objname << "'." << endl;
		return 1;
	}
}
