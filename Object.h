/*
 * Object.h
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <boost/filesystem.hpp>

#define ACL_MANAGER "data-acl"
#define DATA_DIR "data"
#define DEFAULT_PERMISSIONS "rwxpv"
#define USER_DELIMITER '.'
#define GROUP_DELIMITER '\t'

/* Represents an object stored (or to-be-stored) in the object store. Provides
 * methods to store and retrieve objects and ACL details of those objects.
 */
class Object
{
public:
	Object(std::string, std::string);
	// Returns true if the object has been stored.
	bool exists();
	// Stores the object with the given string.
	int put(std::string);
	/* Retrieves the object, populating the given reference string with its
	 * contents.
	 */
	int get(std::string &);
	// Sets the ACL of the object to be the given string.
	int setACL(std::string);
	/* Retrieves the ACL of the object, populating the given reference string
	 * with its contents.
	 */
	int getACL(std::string &);
	/* Returns true if the given user (first string) has the
	 * read|write|execute|permissions|view access specified by the given
	 * character as a member of the given group (second string)
	 */
	bool testACL(std::string, std::string, char);

private:
	std::string owner, filename;
	Object *ACL;
	boost::filesystem::path objpath;
};

#endif /* OBJECT_H_ */
