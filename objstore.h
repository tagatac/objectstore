#include <string>
#include <boost/regex.hpp>
#include <tclap/Constraint.h>
#include <iostream>

#define DEFAULT_REGEX "[\\w\\d_]+"
#define DEFAULT_REGEX_DESCRIPTION "Can contain letters, digits, and underscores; no other characters are legal."
#define OBJNAME_REGEX "[\\w\\d_]+(\\+[\\w\\d_]+)?"

class RegexConstraint : public TCLAP::Constraint<std::string>
{
public:
	RegexConstraint(std::string typeDesc, std::string exp = DEFAULT_REGEX, std::string desc = DEFAULT_REGEX_DESCRIPTION)
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

protected:
	std::string _typeDesc, _exp, _desc;
};

void putObject(std::string, std::string, std::string);
int getObject(std::string, std::string, std::string);
void listObjects(std::string, std::string, bool);
