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
		TCLAP::CmdLine cmd("objput - Reads data from stdin and stores it in an object.", ' ');
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

	put_object(username, groupname, objname);
}

void put_object(string username, string groupname, string objname)
{
	string line;

	// Prompt the user to enter data
	cout << "Please enter file content:" << endl;

	// Create the directory tree down to the user's directory
	fs::path objdir("data");
	objdir /= username;
	fs::create_directories(objdir);

	// Transfer the data from stdin to the file
	fs::path objpath(objdir);
	objpath /= objname;
	ofstream object(objpath.c_str());
	while (getline(cin, line)) object << line << endl;
}
