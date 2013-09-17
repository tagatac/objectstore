/*
 * Object.h
 *
 *  Created on: Sep 17, 2013
 *      Author: tag
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>

class Object
{
public:
	Object(std::string, std::string);
	int put(), get();
	//int put(), get(), setACL(), getACL();
	bool testACL(std::string, std::string, char);

private:
	std::string owner, filename;
	Object *ACL;
};

#endif /* OBJECT_H_ */
