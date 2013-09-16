// objlist.c++

#include <objstore.h>
#include <string>
#include <iostream>
#include <tclap/CmdLine.h>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	string username, groupname;
	bool metadata;

	// parse the commandline with TCLAP
	try
	{
		TCLAP::CmdLine cmd("objlist - Lists all of the objects belonging to the specified user.", ' ');
		TCLAP::SwitchArg metadataSwitch("l", "long", "Use a long listing format", cmd, false);
		RegexConstraint groupnameConstraint("groupname");
		TCLAP::ValueArg<string> groupnameArg("g", "groupname", "Group name", true, "", &groupnameConstraint);
		cmd.add(groupnameArg);
		RegexConstraint usernameConstraint("username");
		TCLAP::ValueArg<string> usernameArg("u", "username", "User's name", true, "", &usernameConstraint);
		cmd.add(usernameArg);
		cmd.parse(argc, argv);
		username = usernameArg.getValue();
		groupname = groupnameArg.getValue();
		metadata = metadataSwitch.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId()
		     << endl;
	}

	return listObjects(username, groupname, metadata);
}

int listObjects(string username, string groupname, bool metadata)
{
	fs::path userpath = fs::current_path();
	userpath /= "data";
	userpath /= username;

	/* Code for iterating through the directory contents mostly taken from
	 * http://www.boost.org/doc/libs/1_54_0/libs/filesystem/example/simple_ls.cpp
	 */

	// Check that user's directory exists
	if (!fs::is_directory(userpath))
		cout << "This user has no objects stored." << endl;
	else
	{
		/* Iterate through the contents of the directory, printing the
		 * name of each file encountered
		 */
		fs::directory_iterator end_iter;
		for (fs::directory_iterator dir_itr(userpath);
		     dir_itr != end_iter; ++dir_itr)
		{
			if (metadata)
				cout << '(' << fs::file_size(dir_itr->path())
				     << "B) ";
			cout << dir_itr->path().filename().string() << endl;
		}
	}

	return 0;
}
