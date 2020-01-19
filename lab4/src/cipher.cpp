#include <cstdlib>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <cstring>
#include <iostream>

using namespace std;

#define BUFFER_SIZE 512

int main (int argc, char * argv[]) {
	
	// Check number of the arguments
    if (argc != 4) {
		cout << "Error: Wrong number of parameters" << endl;
		return -1;
	}

    OpenSSL_add_all_ciphers ();
	
	// Load the public key
	FILE * file_pub = fopen (argv[2], "r");
	if (!file_pub) {
		cout << "Error: invalid public key" << endl;
		return -1;
	}
	EVP_PKEY * publicKey = PEM_read_PUBKEY (file_pub, NULL, NULL, NULL);
	fclose (file_pub);
	
	// Initialize encrypted key
	unsigned char * encryptKey = (unsigned char *) malloc (EVP_PKEY_size(publicKey));
	int encryptKeyLength;
	unsigned char iv [EVP_MAX_IV_LENGTH];
	EVP_CIPHER_CTX ctx;

	// Load a cipher
	const EVP_CIPHER * cipher = EVP_aes_128_cbc();
	const char * cipherString = "AES", * cipherModeString = "CBC";
	const int keyBitLength = 128;

	// Process SealInit
	if (!EVP_SealInit ( &ctx, cipher, &encryptKey, &encryptKeyLength, iv, &publicKey, 1)) {
		cout << "Error: can not process SealInit" << endl;
		return -1;
	}
	
	// Initialize main variables used for ciphering
	unsigned char inBuffer [BUFFER_SIZE];
	unsigned char outBuffer [BUFFER_SIZE];
	int dataLength = 0;
	int outputLength = 0;
	
	// Check files validity
	FILE * file_input = fopen (argv[1], "r");
	if (!file_input) {
		cout << "Error: invalid data file" << endl;
		return -1;
	}

	FILE * file_output = fopen ("tmp", "w");
	if (!file_output) {
		cout << "Error: can not create output file" << endl;
		return -1;
	}
	
	int tmp;
	// Cipher data file
	while ((tmp = fread (inBuffer, sizeof(unsigned char), BUFFER_SIZE, file_input)) > 0) {
		if (!EVP_SealUpdate (&ctx, outBuffer, &outputLength, inBuffer, tmp)) {
			cout << "Error: can not process SealUpdate" << endl;
			return -1;
		}
		fwrite (outBuffer, sizeof(unsigned char), outputLength, file_output);
		dataLength += outputLength;
	}

	// Check data ciphering
	if (!dataLength) {
		cout << "Error: can not read data file" << endl;
		return -1;
	}
	
	// Process SealFinal
	if(!EVP_SealFinal (&ctx, outBuffer, &outputLength)) {
		cout << "Error: can not process SealFinal" << endl;
		return -1;
	}
	fwrite (outBuffer, sizeof(unsigned char), outputLength, file_output);
	dataLength += outputLength;
	
	fclose (file_input);
	fclose (file_output);

	// Save all of the necessary data to the new file
	file_output = fopen (argv[3],"w");
	
	fprintf (file_output, "%s %s %d %d\n", cipherString, cipherModeString, keyBitLength, EVP_MAX_IV_LENGTH);

	fwrite (iv, sizeof(unsigned char), EVP_MAX_IV_LENGTH, file_output);
	fwrite (encryptKey, sizeof(unsigned char), encryptKeyLength, file_output);
	
	fprintf (file_output, "%d\n", dataLength);

	file_input = fopen ("tmp","r");

	while ((tmp = fread (inBuffer, sizeof(unsigned char), BUFFER_SIZE, file_input)))
		fwrite (inBuffer, sizeof(unsigned char), tmp, file_output);
	
	fclose (file_input);
	free (encryptKey);
	fclose(file_output);

	return 0;
}
