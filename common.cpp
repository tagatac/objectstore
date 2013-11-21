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
	int i, stringLength = strlen(reinterpret_cast<char *>(hashedChars));
	char hashedHex[stringLength * 2 + 1];
	for (i=0; i<stringLength; i++)
		sprintf(hashedHex + i * 2, "%02x", hashedChars[i]);
	hashedHex[i*2] = 0;
	cout << "hashedHex: " << hashedHex << endl;
	return hashedHex;
}

void dehexify(string hexString, unsigned char *dehexedString)
{
	// Method inspired by http://stackoverflow.com/a/13344256
	int i;
	for (i=0; i<hexString.length(); i++)
		dehexedString[i] = (char) stoi(hexString.substr(i*2, 2), NULL, 16);
	dehexedString[i] = 0;
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
				  ciphertext[(plaintext_len / AESBLOCK + 2) * AESBLOCK];

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
			boost::lexical_cast<string>(plaintext_len + 1) + '\n');

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
	EVP_cleanup();
	ERR_free_strings();

	ciphertext[ciphertext_len] = '\0';
	string ciphertextString(reinterpret_cast<char *>(ciphertext), ciphertext_len);
	return ciphertextString;
}

string decrypt(string ciphertextString, Object *thisObject)
{
	int len, plaintext_len, ciphertext_len = ciphertextString.length();
	unsigned char iv[AESBLOCK], key[AESBLOCK], *ciphertext, plaintext[ciphertext_len];

	dehexify(thisObject->getIV(), iv);
	dehexify(thisObject->getKey(), key);
	ciphertext = reinterpret_cast<unsigned char *>(const_cast<char *>(ciphertextString.c_str()));

	// EVP initialization
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);

	// Do the decryption - largely copied from
	// http://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
	EVP_CIPHER_CTX *ctx;
	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleCryptoErrors();
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
	    handleCryptoErrors();
	if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
	    handleCryptoErrors();
	plaintext_len = len;
	if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		handleCryptoErrors();
	plaintext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	EVP_cleanup();
	ERR_free_strings();

	plaintext[plaintext_len] = '\0';
	string plaintextString(reinterpret_cast<char *>(plaintext), plaintext_len);
	return plaintextString;
}

bool testKey(unsigned char *key, Object *thisObject)
{
	return hexify(key) == thisObject->getKey();
}
