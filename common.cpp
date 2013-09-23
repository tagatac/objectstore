/*
 * common.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: tag
 */

#include "common.h"
#include "RegexConstraint.h"
#include <string>
#include <tclap/CmdLine.h>
using namespace std;

void defaultCmdLine(string &username, string &groupname,
		    string &objname, string desc, int argc,
		    char *argv[])
{
	try
	{
		TCLAP::CmdLine cmd(desc, ' ');
		RegexConstraint groupnameConstraint("groupname");
		TCLAP::ValueArg<string> groupnameArg("g", "groupname", "Group name",
											 true, "", &groupnameConstraint);
		cmd.add(groupnameArg);
		RegexConstraint usernameConstraint("username");
		TCLAP::ValueArg<string> usernameArg("u", "username", "User's name",
											true, "", &usernameConstraint);
		cmd.add(usernameArg);
		RegexConstraint objnameConstraint("objname", OBJNAME_REGEX);
		TCLAP::UnlabeledValueArg<string> objnameArg("objname", "Object name",
													true, "",
													&objnameConstraint);
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		username = usernameArg.getValue();
		groupname = groupnameArg.getValue();
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}
}

void parseObjname(string username, string objname, string &owner, string &filename)
{
	size_t pos = objname.find(OWNER_DELIMITER);
	if (pos == string::npos)
	{ // delimiter not found -> whole objname is the filename
		owner = username;
		filename = objname;
	}
	else
	{ // delimiter found -> parse out the owner name
		owner = objname.substr(0, pos);
		filename = objname.substr(pos + 1);
	}
}
