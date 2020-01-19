#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <iostream>

using namespace std; 

//Shell script which will output the texts whom sha256 cipher will start with bytes aabb 
//i=0; while true; do echo -n $i | sha256sum | grep -q '^aabb' && echo $i $(echo -n $i | sha256sum); ((i++)); done

int main () {

    //Open text
    char text[] = "116016";
    //Hash function
    char hashFunction[] = "sha256";
    //Initializing important variables to cipher
    EVP_MD_CTX ctx;
    const EVP_MD *type;
    unsigned char hash[EVP_MAX_MD_SIZE];
    int length;

    OpenSSL_add_all_digests();
    type = EVP_get_digestbyname ( hashFunction );
    //Control that decide whether a cipher is valid or invalid
    if ( !type ) {
        cout << "Hash " << hashFunction << " neexistuje" << endl;
        return 1;
    }
    //Ciphering the open text
    EVP_DigestInit ( &ctx, type );
    EVP_DigestUpdate ( &ctx, text, strlen ( text ) );
    EVP_DigestFinal ( &ctx, hash, ( unsigned int * ) &length );
    //Print the final cipher text
    cout << "Hash textu \"" << text << "\" je: "; 
    for ( int i = 0; i < length; i++ ) {
        printf ( "%02x", hash[i] );
    }
    cout << endl;
 
    return 0;
}
