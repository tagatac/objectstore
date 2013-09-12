#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	// Check for correct parameters
	if (argc < 3 || (string) argv[1] != "-u")
	{
		cerr << "Usage: " << argv[0] << " -u username [-l]" << endl;
		return 1;
	}
	bool metadata = false;
	if (argc >= 4 && (string) argv[3] == "-l") metadata = true;

	fs::path userpath = fs::current_path();
	userpath /= "data";
	userpath /= argv[2];

	/* Code for iterating through the directory contents mostly taken from
	 * http://www.boost.org/doc/libs/1_54_0/libs/filesystem/example/simple_ls.cpp
	 */

	// Check that user's directory exists
	if (!fs::is_directory(userpath))
		cout << "This user has no objects stored." << endl;
	else
	{
		/* Iterate through the contents of the directory, printing the
		 * name of each file encountered
		 */
		fs::directory_iterator end_iter;
		for (fs::directory_iterator dir_itr(userpath);
		     dir_itr != end_iter; ++dir_itr)
		{
			if (metadata)
				cout << '(' << fs::file_size(dir_itr->path())
				     << "B) ";
			cout << dir_itr->path().filename().string() << endl;
		}
	}

	return 0;
}
