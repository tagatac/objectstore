// objstore.h

#include <string>
#include <iostream>
#include <boost/regex.hpp>
#include <tclap/CmdLine.h>
#include <tclap/Constraint.h>

#define DEFAULT_REGEX "[\\w\\d_]+"
#define DEFAULT_REGEX_DESCRIPTION "Can contain letters, digits, and underscores; no other characters are legal."
#define OBJNAME_REGEX "[\\w\\d_]+(\\+[\\w\\d_]+)?"

/* Implementation of the TCLAP::Constraint interface to enable regex matching of
 * commandline values
 */
class RegexConstraint : public TCLAP::Constraint<std::string>
{
	std::string _typeDesc, _exp, _desc;

public:
	RegexConstraint(std::string typeDesc, std::string exp = DEFAULT_REGEX,
			std::string desc = DEFAULT_REGEX_DESCRIPTION)
	: _typeDesc(typeDesc),
	  _exp(exp),
	  _desc(desc)
	{}

	bool check(const std::string& val) const
	{
		boost::regex e(_exp);
		return boost::regex_match(val, e);
	}

	std::string description() const
	{
		return _desc;
	}

	std::string shortID() const
	{
		return _typeDesc;
	}
};

class Object
{
public:
	std::string owner, filename;
	Object *ACL;

	Object(std::string o, std::string f)
	{
		owner = o;
		filename = f;
		if (owner != "data-acl")
		{
			Object thisACL("data-acl", owner+'+'+filename);
			ACL = &thisACL;
		}
	}
};

/* Parse the most common syntax commandline with TCLAP
 * (-u username -g groupname objname)
 */
void defaultCmdLine(std::string &username, std::string &groupname,
		    std::string &objname, std::string desc, int argc,
		    char *argv[])
{
	try
	{
		TCLAP::CmdLine cmd(desc, ' ');
		RegexConstraint groupnameConstraint("groupname");
		TCLAP::ValueArg<std::string> groupnameArg("g", "groupname",
				"Group name", true, "", &groupnameConstraint);
		cmd.add(groupnameArg);
		RegexConstraint usernameConstraint("username");
		TCLAP::ValueArg<std::string> usernameArg("u", "username",
				"User's name", true, "", &usernameConstraint);
		cmd.add(usernameArg);
		RegexConstraint objnameConstraint("objname", OBJNAME_REGEX);
		TCLAP::UnlabeledValueArg<std::string> objnameArg("objname",
				"Object name", true, "", &objnameConstraint);
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		username = usernameArg.getValue();
		groupname = groupnameArg.getValue();
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId()
			  << std::endl;
	}
}

void parseObjname(std::string objname, std::string &owner,
		  std::string &filename)
{

}

int putObject(Object *, std::istream *);
int getObject(Object *, std::ofstream *);
int listObjects(std::string, std::string, bool);
int setACL(Object *);
int getACL(Object *);
bool testACL(std::string, std::string, Object *, char);
