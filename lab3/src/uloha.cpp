#include <stdlib.h>
#include <openssl/evp.h>
#include <string>
#include <iostream>

using namespace std;

string filename = "src/Mad_scientist";

int main () {
    //--------------------------------------------------
    string mode;
    cin >> mode;
    unsigned char key[EVP_MAX_KEY_LENGTH] = "key";
    unsigned char iv[EVP_MAX_IV_LENGTH] = "vector";
    const EVP_CIPHER * cipher;
    OpenSSL_add_all_ciphers();
    
    if (mode == "ecb")
        cipher = EVP_aes_128_ecb();
    else if (mode == "cbc")
        cipher = EVP_aes_128_cbc();
    else {
        cout << "Invalid mode!" << endl;
        return 1;
    }
    
    if (!cipher) {
        cout << "Cipher does not exist!" << endl;
        return 1;
    }

    EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
    //--------------------------------------------------
    FILE * input = fopen ((filename + ".bmp").c_str(),"r");
    if (!input) {
        cout << "Invalid file name!" << endl;
        return 1;
    }

    int ctLength = 0;
    int otLength = 0;

    size_t magicNumber = 0, fileSize = 0, reserved = 0, begin = 0;

    fread(&magicNumber, sizeof(unsigned char), 2, input);
    fread(&fileSize, sizeof(unsigned char), 4, input);
    fread(&reserved, sizeof(unsigned char), 4, input);
    fread(&begin, sizeof(unsigned char), 4, input);

    size_t restFileSize = begin - 14;
    otLength = fileSize - begin;
    
    unsigned char * restFile = (unsigned char *) malloc ( restFileSize );
    fread(restFile, sizeof(unsigned char), restFileSize, input);

    unsigned char * ot = (unsigned char *) malloc (otLength * 2);
    fread (ot, sizeof (unsigned char), fileSize, input);
    //--------------------------------------------------
    FILE * output = fopen ((filename + "_" + mode + ".bmp").c_str(),"w");
    fwrite(&magicNumber, sizeof(unsigned char), 2, output );
    fwrite(&fileSize, sizeof(unsigned char), 4, output );
    fwrite(&reserved, sizeof(unsigned char), 4, output );
    fwrite(&begin, sizeof(unsigned char), 4, output );
    fwrite(restFile, sizeof(unsigned char), restFileSize, output);

    EVP_EncryptInit( ctx, cipher, key, iv);
    unsigned char * ct = (unsigned char *) malloc (otLength * 2);
    EVP_EncryptUpdate( ctx, ct, &ctLength, ot, otLength);
    fwrite(ct, sizeof(unsigned char), ctLength, output);

    int temp = 0;
    EVP_EncryptFinal( ctx, ct, &temp);
    fwrite(ct, sizeof(unsigned char), temp, output);
    
    ctLength += temp;
    
    fclose(output);
    fclose(input);
    //--------------------------------------------------
    EVP_DecryptInit(ctx, cipher, key, iv);
    input = fopen ((filename + "_" + mode + ".bmp").c_str(),"r");
    output = fopen ((filename + "_" + mode + "_deciphered" + ".bmp").c_str(),"w");

    fread(&magicNumber, sizeof(unsigned char), 2, input);
    fread(&fileSize, sizeof(unsigned char), 4, input);
    fread(&reserved, sizeof(unsigned char), 4, input);
    fread(&begin, sizeof(unsigned char), 4, input);

    fread(restFile, sizeof(unsigned char), restFileSize, input);

    fwrite(&magicNumber, sizeof(unsigned char), 2, output );
    fwrite(&fileSize, sizeof(unsigned char), 4, output );
    fwrite(&reserved, sizeof(unsigned char), 4, output );
    fwrite(&begin, sizeof(unsigned char), 4, output );
    fwrite(restFile, sizeof(unsigned char), restFileSize, output);
    
    fread(ct, sizeof(unsigned char), ctLength, input);
    
    
    EVP_DecryptUpdate(ctx, ot, &otLength, ct, ctLength);
    fwrite(ot, sizeof(unsigned char), otLength, output);
    
    EVP_DecryptFinal(ctx, ot, &otLength);
    fwrite(ot, sizeof(unsigned char), otLength, output);
    
    fclose(output);
    fclose(input);
    
    free(restFile);
    free(ot);
    free(ct);
    return 0;
}