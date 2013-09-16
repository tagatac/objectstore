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
		RegexConstraint groupnameConstraint("groupname");
		TCLAP::ValueArg<string> groupnameArg("g", "groupname", "Group name", true, "", &groupnameConstraint);
		cmd.add(groupnameArg);
		RegexConstraint usernameConstraint("username");
		TCLAP::ValueArg<string> usernameArg("u", "username", "User's name", true, "", &usernameConstraint);
		cmd.add(usernameArg);
		RegexConstraint objnameConstraint("objname", "[\\w\\d_]+(\\+[\\w\\d_]+)?");
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

	return getObject(username, groupname, objname);
}

int getObject(string username, string groupname, string objname)
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
