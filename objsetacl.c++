// objsetacl.c++

#include <objstore.h>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;

int main(int argc, char **argv)
{
	string username, groupname, objname, owner, filename;
	string desc("objsetacl - Reads an ACL from stdin and sets it.");
	defaultCmdLine(username, groupname, objname, desc, argc, argv);

	parseObjname(objname, owner, filename);
	Object thisObject(owner, filename);
	if (testACL(username, groupname, &thisObject, 'p'))
		setACL(&thisObject);
}

int setACL(Object *)
{
	string line;

	// Prompt the user to enter data
	cout << "Please enter an ACL line or Ctrl+D to quit: ";

//	while 
//	{

	cout << endl;

	return 0;
}
