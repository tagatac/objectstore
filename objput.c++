// objput - Reads data from stdin and stores it in an object.

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

	// Prompt the user to enter data
	cout << "Please enter file content:" << endl;

	// Create the directory tree down to the user's directory
	fs::path objdir("data");
	objdir /= argv[2];
	fs::create_directories(objdir);

	// Transfer the data from stdin to the file
	fs::path objpath(objdir);
	objpath /= argv[3];
	ofstream object(objpath.c_str());
	while (getline(cin, line)) object << line << endl;

	return 0;
}
