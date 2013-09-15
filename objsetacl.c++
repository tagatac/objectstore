#include <string>
#include <iostream>
#include <tclap/CmdLine.h>
using namespace std;

int main(int argc, char **argv)
{
	// parse the commandline with TCLAP
	try
	{
		TCLAP::CmdLine cmd("objsetacl - Reads an ACL from stdin and sets it.", ' ');
		TCLAP::ValueArg<string> groupnameArg("g", "groupname", "Group name", true, "", "groupname");
		cmd.add(groupnameArg);
		TCLAP::ValueArg<string> usernameArg("u", "username", "User's name", true, "", "username");
		cmd.add(usernameArg);
		TCLAP::UnlabeledValueArg<string> objnameArg("objname", "Object name", true, "", "objname");
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		string username = usernameArg.getValue();
		string groupname = groupnameArg.getValue();
		string objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId()
		     << endl;
	}



	return 0;
}
