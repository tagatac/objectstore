/*
 * common.h
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>

#define USERFILE "userfile"
#define OBJNAME_REGEX "[\\w\\d_]+(\\+[\\w\\d_]+)?"
#define USERFILE_DELIMITER ' '
#define OWNER_DELIMITER '+'

/* Parse the most common syntax commandline with TCLAP
 * (-u username -g groupname objname)
 */
void defaultCmdLine(std::string &, std::string, int, char *[]);

/* Returns true if user (first string) belongs to group (second string).  Prints
 * an error message in addition to returning false if not.
 */
bool userfileTest(std::string, std::string);

// Parse owner name out of objname on '+' if necessary
void parseObjname(std::string, std::string &, std::string &);

#endif /* COMMON_H_ */
