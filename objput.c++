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
		RegexConstraint groupnameConstraint("groupname");
		TCLAP::ValueArg<string> groupnameArg("g", "groupname", "Group name", true, "", &groupnameConstraint);
		cmd.add(groupnameArg);
		RegexConstraint usernameConstraint("username");
		TCLAP::ValueArg<string> usernameArg("u", "username", "User's name", true, "", &usernameConstraint);
		cmd.add(usernameArg);
		RegexConstraint objnameConstraint("objname", OBJNAME_REGEX);
		TCLAP::UnlabeledValueArg<string> objnameArg("objname", "Object name", true, "", &objnameConstraint);
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

	putObject(username, groupname, objname);
}

void putObject(string username, string groupname, string objname)
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
