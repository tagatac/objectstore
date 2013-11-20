/*
 * common.h
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>

#define USERFILE "/etc/passwd"
#define OBJNAME_REGEX "[\\w\\d_]+(\\+[\\w\\d_]+)?"
#define USERFILE_DELIMITER ':'
#define OWNER_DELIMITER '+'

/* Parse the most common syntax commandline with TCLAP
 * (objname)
 */
void defaultCmdLine(std::string &, std::string, int, char *[]);

/* Parse the second most common syntax commandline with TCLAP
 * (-k passphrase objname)
 */
void authCmdLine(std::string &, std::string &, std::string, int, char *[]);

// Parse owner name out of objname on '+' if necessary
void parseObjname(std::string, std::string &, std::string &);

#endif /* COMMON_H_ */
