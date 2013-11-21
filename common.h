/*
 * common.h
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "Object.h"
#include <string>

#define USERFILE "/etc/passwd"
#define OBJNAME_REGEX "[\\w\\d_]+(\\+[\\w\\d_]+)?"
#define USERFILE_DELIMITER ':'
#define OWNER_DELIMITER '+'
#define AESBLOCK 16

/* Parse the most common syntax commandline with TCLAP
 * (objname)
 */
void defaultCmdLine(std::string &, std::string, int, char *[]);

/* Parse the second most common syntax commandline with TCLAP
 * (-k passphrase objname)
 */
void authCmdLine(std::string &, std::string &, std::string, int, char *[]);

// Convert MD5 hash of passphrase to a hex string (for storage)
std::string hexify(unsigned char *);

// Convert hex string to ASCII string
void dehexify(std::string, unsigned char *);

// Parse owner name out of objname on '+' if necessary
void parseObjname(std::string, std::string &, std::string &);

// Encrypt the contents of an object, saving the IV and filesize first
std::string encrypt(std::string, unsigned char *, Object *);

// Decrypt an object
std::string decrypt(std::string, Object *);

// Test if the supplied passphrase is correct
bool testKey(unsigned char *, Object *);

#endif /* COMMON_H_ */
