// objput.c++

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
	string desc("objput - Reads data from stdin and stores it in an object.");
	defaultCmdLine(username, groupname, objname, desc, argc, argv);

	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (testACL(username, groupname, &thisObject, 'w'))
	{
		// Prompt the user to enter data
		cout << "Please enter file content:" << endl;
		// Create the object on disk
		return putObject(&thisObject, cin);
	}
}

int putObject(Object *thisObject, ifstream *thisIFStream)
{
	string line;

	// Create the directory tree down to the user's directory
	fs::path objdir("data");
	objdir /= thisObject->owner;
	fs::create_directories(objdir);

	// Transfer the data from stdin to the file
	fs::path objpath(objdir);
	objpath /= thisObject->filename;
	ofstream objectstream(objpath.c_str());
	while (getline(thisIFStream, line)) objectstream << line << endl;

	return 0
}
