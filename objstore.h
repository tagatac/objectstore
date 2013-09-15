#include <string>
#include <boost/regex.hpp>
#include <tclap/Constraint.h>
#include <iostream>

class UsernameConstraint : public TCLAP::Constraint<std::string>
{
	bool check(const std::string& val) const
	{
		std::string estring = "[\\w\\d_]+";
		boost::regex e(estring);
		return boost::regex_match(val, e);
	}

	std::string description() const
	{
		return "Can contain letters, digits, and underscores; no other characters are legal.";
	}

	std::string shortID() const
	{
		return "objname";
	}
};

typedef UsernameConstraint GroupnameConstraint;

class ObjnameConstraint : public TCLAP::Constraint<std::string>
{
	bool check(const std::string& val) const
	{
		std::string estring = "[\\w\\d_]+(\\+[\\w\\d_]+)?";
		boost::regex e(estring);
		return boost::regex_match(val, e);
	}

	std::string description() const
	{
		return "Can contain letters, digits, and underscores; no other characters are legal.";
	}

	std::string shortID() const
	{
		return "objname";
	}
};

void putObject(std::string, std::string, std::string);
int getObject(std::string, std::string, std::string);
void listObjects(std::string, std::string, bool);
