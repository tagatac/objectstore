/*
 * common.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: David Tagatac
 */

#include "common.h"
#include "RegexConstraint.h"
#include <string>
#include <tclap/CmdLine.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
using namespace std;
namespace fs = boost::filesystem;

void defaultCmdLine(string &objname, string desc, int argc, char *argv[])
{
	try
	{
		TCLAP::CmdLine cmd(desc, ' ');
		RegexConstraint objnameConstraint("objname", OBJNAME_REGEX);
		TCLAP::UnlabeledValueArg<string> objnameArg("objname", "Object name",
													true, "",
													&objnameConstraint);
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}
}

void authCmdLine(string &passphrase, string &objname, string desc, int argc, char *argv[])
{
	try
	{
		TCLAP::CmdLine cmd(desc, ' ');
		TCLAP::ValueArg<string> passphraseArg("k", "passphrase",
											  "Encryption pass phrase", true,
											  "", "passphrase");
		cmd.add(passphraseArg);
		RegexConstraint objnameConstraint("objname", OBJNAME_REGEX);
		TCLAP::UnlabeledValueArg<string> objnameArg("objname", "Object name",
													true, "",
													&objnameConstraint);
		cmd.add(objnameArg);
		cmd.parse(argc, argv);
		passphrase = passphraseArg.getValue();
		objname = objnameArg.getValue();
	}
	catch (TCLAP::ArgException &e)
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}
}

void parseObjname(string objname, string &owner, string &filename)
{
	string username = boost::lexical_cast<string>(getuid());
	size_t pos = objname.find(OWNER_DELIMITER);
	if (pos == string::npos)
	{ // delimiter not found -> whole objname is the filename
		owner = username;
		filename = objname;
	}
	else
	{ // delimiter found -> parse out the owner name
		owner = objname.substr(0, pos);
		filename = objname.substr(pos + 1);
	}
}
