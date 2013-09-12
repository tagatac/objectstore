#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;

int main(int argc, char *argv[])
{
	// Check for correct parameters
	if (argc < 4 || (string) argv[1] != "-u")
	{
		cerr << "Usage: " << argv[0] << " -u username objname" << endl;
		return 1;
	}

	string contents, objdir;
	ofstream object;

	cout << "Please enter file content:" << endl;
	getline(cin, contents);
	objdir = "data/" + (string) argv[2];
	boost::filesystem::create_directories(objdir);
	object.open((objdir + '/' + (string) argv[3]).c_str());
	object << contents;
	object.close();

	return 0;
}
