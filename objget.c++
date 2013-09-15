#include <objstore.h>
#include <iostream>
#include <string>
#include <fstream>
#include <tclap/CmdLine.h>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	string username, groupname, objname;

	// parse the commandline with TCLAP
	try
	{
		TCLAP::CmdLine cmd("objget - Writes the contents of a retrieved object to stdout.", ' ');
		TCLAP::ValueArg<string> groupnameArg("g", "groupname", "Group name", true, "", "groupname");
		cmd.add(groupnameArg);
		TCLAP::ValueArg<string> usernameArg("u", "username", "User's name", true, "", "username");
		cmd.add(usernameArg);
		TCLAP::UnlabeledValueArg<string> objnameArg("objname", "Object name", true, "", "objname");
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		username = usernameArg.getValue();
		groupname = groupnameArg.getValue();
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId()
		     << endl;
	}

	return get_object(username, groupname, objname);
}

int get_object(string username, string groupname, string objname)
{
	string line;

	// Open the file
	fs::path objpath("data");
	objpath /= username;
	objpath /= objname;
	ifstream object(objpath.c_str());
	if (!object)
	{
		cerr << "Invalid file: " << objpath << endl;
		return 1;
	}
	// Write the contents of the file to stdout
	else
		while (getline (object, line)) cout << line << endl;

	return 0;
}
