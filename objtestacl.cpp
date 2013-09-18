// objtestacl.cpp

#include "common.h"
#include "Object.h"
#include "RegexConstraint.h"
#include <string>
#include <iostream>
#include <tclap/CmdLine.h>
#include <boost/filesystem.hpp>
using namespace std;

int main(int argc, char *argv[])
{
	string username, groupname, objname, owner, filename;
	char access;

	// parse the commandline with TCLAP
	try
	{
		TCLAP::CmdLine cmd("objtestacl - tests file permissions and outputs the single word 'allowed' or 'denied'.", ' ');
		RegexConstraint accessConstraint("access", "[rwxpv]",
						 "r|w|x|p|v");
		TCLAP::ValueArg<string> accessArg("a", "access",
				"Access required", true, "", &accessConstraint);
		cmd.add(accessArg);
		RegexConstraint groupnameConstraint("groupname");
		TCLAP::ValueArg<string> groupnameArg("g", "groupname",
				"Group name", true, "", &groupnameConstraint);
		cmd.add(groupnameArg);
		RegexConstraint usernameConstraint("username");
		TCLAP::ValueArg<string> usernameArg("u", "username",
				"User's name", true, "", &usernameConstraint);
		cmd.add(usernameArg);
		RegexConstraint objnameConstraint("objname", OBJNAME_REGEX);
		TCLAP::UnlabeledValueArg<std::string> objnameArg("objname",
				"Object name", true, "", &objnameConstraint);
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		username = usernameArg.getValue();
		groupname = groupnameArg.getValue();
		access = accessArg.getValue().at(1);
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId()
		     << endl;
	}

	parseObjname(username, objname, owner, filename);
	Object thisObject(owner, filename);
	if (!thisObject.exists())
	{
		cerr << owner << "'s file \"" << filename << "\" does not exist." << endl;
		return 1;
	}
	else
		if (thisObject.testACL(username, groupname, access))
			cout << "allowed" << endl;
		else
			cout << "denied" << endl;
}
