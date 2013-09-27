/*
 * RegexConstraint.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#include "RegexConstraint.h"
#include <string>
#include <boost/regex.hpp>
using namespace std;

RegexConstraint::RegexConstraint(string typeDesc,
								 string exp,
								 string desc)
{
	_typeDesc = typeDesc;
	_exp = exp;
	_desc = desc;
}

string RegexConstraint::description() const
{
	return _desc;
}

string RegexConstraint::shortID() const
{
	return _typeDesc;
}

bool RegexConstraint::check(const std::string& val) const
{
	boost::regex e(_exp);
	return boost::regex_match(val, e);
}
