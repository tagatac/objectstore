/*
 * objtestacl.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: David Tagatac
 */

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
	string username, groupname, objname, owner, filename, accesses;

	// parse the commandline with TCLAP
	try
	{
		TCLAP::CmdLine cmd("objtestacl - tests file permissions and outputs the single word 'allowed' or 'denied'.", ' ');
		RegexConstraint accessConstraint("access", "[rwxpv]*", "[r][w][x][p][v]");
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
		accesses = accessArg.getValue();
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}

	parseObjname(username, objname, owner, filename);
	Object thisObject(owner, filename);
	// Check that the user belongs to the group.
	if (!userfileTest(username, groupname))
		return 1;
	// Check that the object is in the object store.
	if (!thisObject.exists())
	{
		cerr << owner << "'s object \"" << filename << "\" does not exist."
			 << endl;
		return 1;
	}
	/* Loop through all of the accesses given, checking if user username has
	 * that access as a member of group groupname.  Answer "denied" if any of
	 * the access tests fail.
	 */
	for(unsigned int i=0; i<accesses.length(); i++)
		if (!thisObject.testACL(username, groupname, accesses.at(i)))
		{
			cout << "denied" << endl;
			return 0;
		}
	// If none of the access tests fail, answer "allowed".
	cout << "allowed" << endl;
}
