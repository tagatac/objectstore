// objget - Writes the contents of a retrieved object to stdout.

#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	// Check for correct parameters
	if (argc < 4 || (string) argv[1] != "-u")
	{
		cerr << "Usage: " << argv[0] << " -u username objname" << endl;
		return 1;
	}

	string line;

	// Open the file
	fs::path objpath("data");
	objpath /= argv[2];
	objpath /= argv[3];
	ifstream object(objpath.c_str());
	if (!object)
	{
		cerr << "Invalid file: " << objpath << endl;
		return 1;
	}
	// Write the contents of the file to stdout
	else
		while (getline (object, line)) cout << line << endl;

	return 0;
}
