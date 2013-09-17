// objput.cpp

#include "common.h"
#include "Object.h"
#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	string username, groupname, objname, owner, filename;
	string desc("objput - Reads data from stdin and stores it in an object.");
	defaultCmdLine(username, groupname, objname, desc, argc, argv);

	parseObjname(username, objname, owner, filename);
	Object thisObject(owner, filename);
	cout << owner;
	if (thisObject.testACL(username, groupname, 'w'))
	{
		// Prompt the user to enter data
		cout << "Please enter file content:" << endl;
		// Create the object on disk
		return thisObject.put();
	}
}
