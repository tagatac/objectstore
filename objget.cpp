/*
 * objget.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: David Tagatac
 */

#include "common.h"
#include "Object.h"
#include <string>
#include <iostream>
#include <openssl/md5.h>
using namespace std;

int main(int argc, char *argv[])
{
	setuid(getuid());
	string passphrase, objname, owner, filename;
	unsigned char key[AESBLOCK];
	string desc = "objget - Writes the contents of a retrieved object to stdout.";
	authCmdLine(passphrase, objname, desc, argc, argv);
	MD5(reinterpret_cast<unsigned char *>(const_cast<char *>(passphrase.c_str())),
					passphrase.length(), key);
	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (thisObject.testACL('r') && testKey(key, &thisObject))
	{
		string contents, encryptedContents;
		int retval;
		if (!(retval = thisObject.get(encryptedContents)))
			contents = decrypt(encryptedContents, &thisObject);
			cout << contents << endl;
		return retval;
	}
	else
	{
		cerr << "You do not have permission to get object '" << objname << "'."
			 << endl;
		return 1;
	}
}
