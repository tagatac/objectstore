// objget.c++

#include <objstore.h>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	string username, groupname, objname, owner, filename;
	string desc("objget - Writes the contents of a retrieved object to stdout.");
	defaultCmdLine(username, groupname, objname, desc, argc, argv);

	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (testACL(username, groupname, &thisObject, 'r'))
		return getObject(&thisObject, cout);
}

int getObject(Object *thisObject, ofstream *thisOFStream)
{
	string line;

	// Open the file
	fs::path objpath("data");
	objpath /= thisObject->owner;
	objpath /= thisObject->filename;
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
