#include <openssl/evp.h>
#include "hexa_byte_convertor.h"

using namespace std;
//Class that handles ciphering and determinating the OT from public OT, public CT and its own CT
class RC4 {
public:
    //Constructor will load two open texts and type of cipher and will prepare all of the necessary variables to be used while ciphering 
    RC4 ( string openPub, string openPri, string cN ) {
        m_Ctx = EVP_CIPHER_CTX_new();
        //Convert C++ string to array of unsigned chars - this format is required by the cipher function
        for ( size_t i = 0; i < openPub.length(); i++ )
            m_OpenPub[i] = (unsigned char) openPub[i];
        for ( size_t i = 0; i < openPri.length(); i++ )
            m_OpenPri[i] = (unsigned char) openPri[i];

        //Preparing necessary variables for ciphering
        const char * cipherName = cN.c_str();
        OpenSSL_add_all_ciphers();
        m_Cipher = EVP_get_cipherbyname ( cipherName );
        m_OpenPubLength = openPub.length();
        m_OpenPriLength = openPri.length();
        m_CiphPubLength = 0;
        m_CiphPriLength = 0;

        //Preparing the initial vector and secret key for ciphering
        string key = "top secret key!!";
        string iv = "initial vector";
        for ( size_t i = 0; i < key.length(); i++ )
            m_Key[i] = (unsigned char) key[i];
        for ( size_t i = 0; i < iv.length(); i++ )
            m_InitialVector[i] = (unsigned char) iv[i];
    }
    //------------
    ~RC4 () {
        EVP_CIPHER_CTX_free ( m_Ctx );
    }
    //------------
    //Function will check whether the cipher name is valid or not
    bool checkCipher () {
        return m_Cipher;
    }
    //------------
    //Function will print loaded open texts
    void printOpenTexts () {
        cout << "Public OT: " << m_OpenPub << endl;
        cout << "Public OT - hex: ";
        for ( int i = 0; i < m_OpenPubLength; i++ )
            printf ( "%02x", m_OpenPub[i] );
        cout << endl;
        cout << "Private OT: " << m_OpenPri << endl;
        cout << "Private OT - hex: ";
        for ( int i = 0; i < m_OpenPriLength; i++ )
            printf ( "%02x", m_OpenPri[i] );
        cout << endl;
    }
    //Function will print ciphered texts
    void printCipherTexts () {
        cout << "Public CT: " << m_CiphPub << endl;
        cout << "Public CT - hex: ";
        for ( int i = 0; i < m_CiphPubLength; i++ )
            printf ( "%02x", m_CiphPub[i] );
        cout << endl;
        cout << "Private CT: " << m_CiphPri << endl;
        cout << "Private CT - hex: ";
        for ( int i = 0; i < m_CiphPriLength; i++ )
            printf ( "%02x", m_CiphPri[i] );
        cout << endl;
    }
    //------------
    //Function will cipher text
    void cipherText () {
        int tmpLength = 0;
        
        EVP_EncryptInit ( m_Ctx, m_Cipher, m_Key, m_InitialVector );
        EVP_EncryptUpdate ( m_Ctx, m_CiphPri, &tmpLength, m_OpenPri, m_OpenPriLength  );
        m_CiphPriLength += tmpLength;
        EVP_EncryptFinal ( m_Ctx, m_CiphPri + m_CiphPriLength, &tmpLength );
        m_CiphPriLength += tmpLength;

        EVP_EncryptInit ( m_Ctx, m_Cipher, m_Key, m_InitialVector );
        EVP_EncryptUpdate ( m_Ctx, m_CiphPub, &tmpLength, m_OpenPub, m_OpenPubLength  );
        m_CiphPubLength += tmpLength;
        EVP_EncryptFinal ( m_Ctx, m_CiphPub + m_CiphPubLength, &tmpLength );
        m_CiphPubLength += tmpLength;
    }
    //------------
    //Function will decipher text
    void decipherText () {
        int tmpLength = 0;

        EVP_DecryptInit ( m_Ctx, m_Cipher, m_Key, m_InitialVector );
        EVP_DecryptUpdate ( m_Ctx, m_OpenPri, &tmpLength, m_CiphPri, m_CiphPriLength );
        m_OpenPriLength += tmpLength;
        EVP_DecryptFinal ( m_Ctx, m_OpenPri + m_OpenPriLength, &tmpLength );
        m_OpenPriLength += tmpLength;

        EVP_DecryptInit ( m_Ctx, m_Cipher, m_Key, m_InitialVector );
        EVP_DecryptUpdate ( m_Ctx, m_OpenPub, &tmpLength, m_CiphPub, m_CiphPubLength );
        m_OpenPubLength += tmpLength;
        EVP_DecryptFinal ( m_Ctx, m_OpenPub + m_OpenPubLength, &tmpLength );
        m_OpenPubLength += tmpLength;
    }
    //------------
    //Function will break cipher text using public OT, public CT and its own CT -> it will output needed OT
    string breakCipher ( string pubOT, string pubCT, string priCT ) {
        vector<uint8_t> bytePubOT = Converter::hexaToByte ( pubOT );
        vector<uint8_t> bytePubCT = Converter::hexaToByte ( pubCT );
        vector<uint8_t> bytePriCT = Converter::hexaToByte ( priCT );

        vector<uint8_t> publicXor = Converter::byteXOR ( bytePubOT, bytePubCT );
        vector<uint8_t> privateXor = Converter::byteXOR ( bytePriCT, publicXor );

        return ( Converter::ByteToASCII ( privateXor ) );
    }

private:
    unsigned char m_OpenPub [1024]; 
    unsigned char m_OpenPri [1024];
    unsigned char m_CiphPub [1024];
    unsigned char m_CiphPri [1024];
    
    unsigned char m_Key [1024];
    unsigned char m_InitialVector [1024];
    const EVP_CIPHER * m_Cipher;

    int m_OpenPubLength, m_OpenPriLength, m_CiphPubLength, m_CiphPriLength;

    EVP_CIPHER_CTX * m_Ctx;
};

int main () {
    //Handler object which will load two open texts and type of cipher
    RC4 handler ( "abcdefghijklmnopqrstuvwxyz0123", "this is top secret message!!!!", "RC4" );
    //Function will check whether the cipher name is valid or invalid
    if ( !handler.checkCipher() ) {
        cout << "Given cipher does not exist!" << endl;
        return 1;
    }
    //Printing open texts and equivalent cipher texts
    handler.printOpenTexts();
    handler.cipherText ();
    cout << "---------------------" << endl;
    cout << "---------------------" << endl;
    handler.printCipherTexts();
    cout << "---------------------" << endl;
    cout << "---------------------" << endl;
    //Function will determinate the OT from public OT, public CT and its own CT
    cout << handler.breakCipher ( "6162636465666768696a6b6c6d6e6f707172737475767778797a30313233", "e0f571ead0ad80a660263f5cd5885bf17fa3411f017306801dd1634db9f1", "f5ff7bfd95a294ee7d232410cb8357f36ba512061176029903ce725daae3" ) << endl;

    return 0;
}