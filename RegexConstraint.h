/*
 * RegexConstraint.h
 *
 * Implementation of the TCLAP::Constraint interface to enable regex matching of
 * commandline values
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#ifndef REGEXCONSTRAINT_H_
#define REGEXCONSTRAINT_H_

#include <tclap/Constraint.h>

#define DEFAULT_REGEX "[\\w\\d_]+"
#define DEFAULT_REGEX_DESCRIPTION "Can contain letters, digits, and underscores; no other characters are legal."

class RegexConstraint : public TCLAP::Constraint<std::string>{
public:
	RegexConstraint(std::string, std::string exp = DEFAULT_REGEX,
					std::string desc = DEFAULT_REGEX_DESCRIPTION);
	std::string description() const;
	std::string shortID() const;
	bool check(const std::string&) const;

private:
	std::string _typeDesc, _exp, _desc;
};

#endif /* REGEXCONSTRAINT_H_ */
