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

string hexify(unsigned char *hashedChars)
{
	// Method inspired by http://stackoverflow.com/a/14050569
	char hashedHex[AESBLOCK * 2 + 1];
	int i;
	for (i=0; i<AESBLOCK; i++)
	{
		sprintf(hashedHex + i * 2, "%02x", hashedChars[i]);
	}
	hashedHex[i*2] = 0;
	return hashedHex;
}

unsigned char *dehexify(string hexString)
{
	// Method inspired by http://stackoverflow.com/a/3221193
	vector<unsigned char> dehexedString;
	istringstream hexStringStream(hexString);
	unsigned int c;

	while (hexStringStream >> hex >> c)
		dehexedString.push_back(c);

	return dehexedString;
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

void handleCryptoErrors()
{
	ERR_print_errors_fp(stderr);
	abort();
}

string encrypt(string plaintextString, unsigned char *key, Object *metadata)
{
	int len, ciphertext_len, plaintext_len = plaintextString.length();
	char ivTemp[AESBLOCK];
	unsigned char *iv, *plaintext,
				  ciphertext[(plaintext_len / AESBLOCK + 1) * AESBLOCK];

	// Obtain an IV from urandom
	fs::ifstream urandom("/dev/urandom");
	if (!urandom)
	{
		cerr << "Encryption error: urandom unavailable." << endl;
		return "";
	}
	urandom.read(ivTemp, AESBLOCK);

	iv = reinterpret_cast<unsigned char *>(ivTemp);
	plaintext = reinterpret_cast<unsigned char *>(const_cast<char *>(plaintextString.c_str()));

	// Write all of the metadata
	metadata->put(hexify(iv) + '\n' + hexify(key) + '\n' +
			boost::lexical_cast<string>(plaintextString.length() + 1) + '\n');

	// EVP initialization
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);

	// Do the encryption - largely copied from
	// http://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
	EVP_CIPHER_CTX *ctx;
	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleCryptoErrors();
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
	    handleCryptoErrors();
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
	    handleCryptoErrors();
	ciphertext_len = len;
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		handleCryptoErrors();
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);

	string ciphertextString(reinterpret_cast<const char *>(ciphertext), ciphertext_len);
	return ciphertextString;
}

string decrypt(string ciphertextString, Object *metadata)
{
	char IV[AESBLOCK/sizeof(char)];
	fs::ifstream urandom("/dev/urandom");
	if (!urandom)
	{
		cerr << "Encryption error: urandom unavailable." << endl;
		return "";
	}

	return "";
}

bool testPass(string passphrase, Object *thisObject)
{
	return hexify(passphrase) == thisObject->getKey();
}
