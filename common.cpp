/*
 * common.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#include "common.h"
#include "RegexConstraint.h"
#include <string>
#include <tclap/CmdLine.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace std;
namespace fs = boost::filesystem;

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

bool userfileTest(string username, string groupname)
{
	fs::path userfilepath(USERFILE);
	string userfileline;

	// Open the userfile
	fs::ifstream userfilestream(userfilepath);
	if (!userfilestream)
	{
		cerr << "Invalid userfile!" << endl;
		return false;
	}
	// Parse each line of the userfile until the user is found.
	while (getline(userfilestream, userfileline))
	{
		// First match the user.
		size_t cursor2, cursor1 = userfileline.find(USERFILE_DELIMITER);
		string user = userfileline.substr(0, cursor1);
		if (user == username)
		{
			// If the user matches, match the group.
			do
			{
				cursor2 = userfileline.find(USERFILE_DELIMITER, cursor1 + 1);
				string group = userfileline.substr(cursor1 + 1, cursor2 - (cursor1 + 1));
				// If the user and the group match, use these permissions.
				if (group == groupname) return true;
				else cursor1 = cursor2;
			}
			while (cursor2 != string::npos);
		}
	}
	/* If, after looking at every line, the user/group pair is not found, this
	 * user does not belong to this group.
	 */
	cerr << "Error: User '" << username << "' does not belong to group '"
		 << groupname << "'." << endl;
	return false;
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
