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
	
	OpenSSL_add_all_ciphers();
	
	// Load the private key
	FILE * file_priv = fopen (argv[2],"r");
	if (!file_priv) {
		cout << "Error: invalid public key" << endl;
		return -1;
	}

	EVP_PKEY * privateKey = PEM_read_PrivateKey(file_priv, NULL, NULL, NULL);
	fclose (file_priv);
	
	// Initialize main variables used for deciphering
	int encryptKeyLength = EVP_PKEY_size (privateKey);
	unsigned char * encryptKey = (unsigned char *) malloc (encryptKeyLength);
	unsigned char iv [EVP_MAX_IV_LENGTH];
	int ivLength;
	int keyBitLength;
	char cipherString [16];
	char cipherModeString [16];
	
	// Open input file
	FILE * file_input = fopen (argv[1],"r");
	if (!file_input) {
		cout << "Error: can not open input file" << endl;
        return -1;
	}
	
	if (fscanf (file_input, "%s %s %d %d\n", cipherString, cipherModeString, &keyBitLength, &ivLength) != 4) {
		cout << "Error: can not read parameters from the file, " << argv[1] << endl;
		return -1;
	}
	
	// Print out the information from the input file
    cout << "==============================" << endl;
    cout << "Cipher: " << cipherString << " [" << cipherModeString << "]" << endl;
    cout << "Length of the key: " << keyBitLength << " bytes" << endl;
    cout << "Length of the IV: " << ivLength << " bytes" << endl;
    cout << "==============================" << endl;
	
	// Load the proper cipher
    const EVP_CIPHER * cipher = EVP_aes_128_cbc ();
    if (!cipher) {
        cout << "Error: invalid cipher" << endl;
        return -1;
    }

	// Check validity of the IV and key
	if (fread (iv, sizeof(unsigned char), ivLength, file_input) != (size_t)ivLength) {
		cout << "Error: can not load IV" << endl;
        return -1;
	}
	if (fread (encryptKey, sizeof(unsigned char), encryptKeyLength, file_input) != (size_t)encryptKeyLength) {
		cout << "Error: can not load the key" << endl;
        return -1;
	}
	
	int encryptLength = 0;
	if (fscanf (file_input, "%d\n", &encryptLength) != 1 || encryptLength <= 0) {
		cout << "Error: can not read the length of encrypted data" << endl;
		return -1;
	}
	
	cout << "Length of the ciphered data: " << encryptLength  << " bytes" << endl;
	
	EVP_CIPHER_CTX ctx;
	
	if (!EVP_OpenInit ( &ctx, cipher, encryptKey, encryptKeyLength, iv, privateKey)) {
		cout << "Error: can not process OpenInit" << endl;
        return -1;
	}
	
	// Decipher data file
	unsigned char inBuffer [BUFFER_SIZE];
	unsigned char outBuffer [BUFFER_SIZE];
	int outLength = 0;
	int dataLength = 0;
	int inputLength = 0;
	
	FILE * file_output = fopen (argv[3],"w");
	if (!file_output) {
		cout << "Error: can not open output data file" << endl;
        return -1;
	}
	
	int tmp;
	// Deciphering data file
	while ((tmp = fread (inBuffer, sizeof(unsigned char), BUFFER_SIZE, file_input)) > 0) {
		if ((inputLength += tmp) > encryptLength)
			break;
		if (!EVP_OpenUpdate(&ctx,outBuffer,&outLength,inBuffer,tmp)) {
			cout << "Error: can not process OpenUpdate" << endl;
			return -1;
		}
		fwrite (outBuffer, sizeof(unsigned char), outLength, file_output);
		dataLength += outLength;
	}

	// Check the validity of the data and its match with deciphered data to binary match
	if (!inputLength) {
		cout << "Error: can not read from the input data file" << endl;
		return -1;
	}
	else if (inputLength != encryptLength) {
		cout << "Error: length of input data differs " << inputLength << " != " << encryptLength << endl;
        return -1;
	}
	
	// Finish deciphering by calling EVP_OpenFinal
	if (!EVP_OpenFinal (&ctx,outBuffer,&outLength)) {
		cout << "Error: can not process OpenFinal" << endl;
		return -1;
	}
	fwrite (outBuffer, sizeof(unsigned char), outLength, file_output);
	dataLength += outLength;
	
	fclose(file_input);
	cout << "Written deciphered file with length: " << dataLength << " bytes" << endl;
	cout << "==============================" << endl;
	
	free (encryptKey);
	fclose(file_output);

	return 0;
}
