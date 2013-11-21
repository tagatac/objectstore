/*
 * objput.cpp
 *
 *  Created on: Sep 11, 2013
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
	string desc = "objput - Reads data from stdin and stores it in an object.";
	authCmdLine(passphrase, objname, desc, argc, argv);
	MD5(reinterpret_cast<unsigned char *>(const_cast<char *>(passphrase.c_str())),
				passphrase.length(), key);
	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (thisObject.testACL('w'))
	{
		// Prompt the user to enter data
		cout << "Please enter file content:" << endl;
		// Put the stream into a string
		string contents;
		istreambuf_iterator<char> eos;
		contents = string(istreambuf_iterator<char>(cin), eos);
		// Encrypt the string
		string encryptedContents = encrypt(contents, key, thisObject.metadata);
		if (encryptedContents.length() == 0) return 1;
		// Create the object on disk
		else return thisObject.put(encryptedContents);
	}
	else
	{
		cerr << "You do not have permission to put object '" << objname << "'."
			 << endl;
		return 1;
	}
}
