/*
 * Object.h
 *
 *  Created on: Sep 17, 2013
 *      Author: tag
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <boost/filesystem.hpp>

class Object
{
public:
	Object(std::string, std::string);
	bool exists();
	int put(std::istream &), get(std::string &);
	bool testACL(std::string, std::string, char);

private:
	std::string owner, filename;
	Object *ACL;
	boost::filesystem::path objpath;
};

#endif /* OBJECT_H_ */
