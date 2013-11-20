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
using namespace std;

int main(int argc, char *argv[])
{
	string passphrase, objname, owner, filename;
	string desc = "objput - Reads data from stdin and stores it in an object.";
	authCmdLine(passphrase, objname, desc, argc, argv);
	cout << "passphrase: " << passphrase << endl;
	return 1;

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
		// Create the object on disk
		return thisObject.put(contents);
	}
	else
	{
		cerr << "You do not have permission to put object '" << objname << "'."
			 << endl;
		return 1;
	}
}
