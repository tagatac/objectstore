/*
 * common.h
 *
 *  Created on: Sep 17, 2013
 *      Author: tag
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>

#define OBJNAME_REGEX "[\\w\\d_]+(\\+[\\w\\d_]+)?"
#define OWNER_DELIMITER '+'

/* Parse the most common syntax commandline with TCLAP
 * (-u username -g groupname objname)
 */
void defaultCmdLine(std::string &, std::string &, std::string &, std::string, int,
					char *[]);

// Parse owner name out of objname on '+' if necessary
void parseObjname(std::string, std::string, std::string &, std::string &);

#endif /* COMMON_H_ */
