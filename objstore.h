#include <string>
#include <regex>
#include <tclap/Constraint.h>

namespace std {

namespace TCLAP {

class ObjnameConstraint : public Constraint<string>
{
	bool Constraint<string>::check(const string& val) const
	{
		regex e("[\\w\\d_]+(\\+[\\w\\d_]+");
		return regex_match(val, e);
	}
}

} //namespace TCLAP

void putObject(std::string, std::string, std::string);
int getObject(std::string, std::string, std::string);
void listObjects(std::string, std::string, bool);

} //namespace std
