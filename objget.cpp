// objget.cpp

#include "common.h"
#include "Object.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	string username, groupname, objname, owner, filename;
	string desc = "objget - Writes the contents of a retrieved object to stdout.";
	defaultCmdLine(username, groupname, objname, desc, argc, argv);

	parseObjname(username, objname, owner, filename);
	Object thisObject(owner, filename);
	if (thisObject.testACL(username, groupname, 'r'))
	{
		string contents;
		int retval;
		if (!(retval = thisObject.get(contents)))
			cout << contents << endl;
		return retval;
	}
	else
	{
		cerr << "User " << username <<
				" does not have permission to get this object." << endl;
		return 1;
	}
}
