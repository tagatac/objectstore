/*
 * objget.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: David Tagatac
 */

#include "common.h"
#include "Object.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	string passphrase, objname, owner, filename;
	string desc = "objget - Writes the contents of a retrieved object to stdout.";
	authCmdLine(passphrase, objname, desc, argc, argv);

	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (thisObject.testACL('r'))
	{
		string contents;
		int retval;
		if (!(retval = thisObject.get(contents)))
			cout << contents << endl;
		return retval;
	}
	else
	{
		cerr << "You do not have permission to get object '" << objname << "'."
			 << endl;
		return 1;
	}
}
