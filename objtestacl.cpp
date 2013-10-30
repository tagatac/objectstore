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
	string objname, owner, filename, accesses;

	// parse the commandline with TCLAP
	try
	{
		TCLAP::CmdLine cmd("objtestacl - tests file permissions and outputs the single word 'allowed' or 'denied'.", ' ');
		RegexConstraint accessConstraint("access", "[rwxpv]*", "[r][w][x][p][v]");
		TCLAP::ValueArg<string> accessArg("a", "access",
				"Access required", true, "", &accessConstraint);
		cmd.add(accessArg);
		RegexConstraint objnameConstraint("objname", OBJNAME_REGEX);
		TCLAP::UnlabeledValueArg<std::string> objnameArg("objname",
				"Object name", true, "", &objnameConstraint);
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		accesses = accessArg.getValue();
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}

	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
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
		if (!thisObject.testACL(accesses.at(i)))
		{
			cout << "denied" << endl;
			return 0;
		}
	// If none of the access tests fail, answer "allowed".
	cout << "allowed" << endl;
}
