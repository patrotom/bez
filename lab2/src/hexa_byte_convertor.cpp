#include "hexa_byte_convertor.h"
//-------------
vector<uint8_t> Converter::hexaToByte ( string str ) {
    vector<uint8_t> byteField;
    for ( size_t i = 1; i <= str.length(); i+=2 ) {
        int value;
        string con = string() + "0x" +  str[i-1] + str[i];
        value = stoi ( con, nullptr, 16 );
        byteField.push_back ( value );
    }
    return byteField;
}
//-------------
vector<uint8_t> Converter::byteStringToByteField ( string str ) {
    vector<uint8_t> byteField;
    for ( size_t i = 1; i <= str.length(); i+=2 ) {
        int value;
        value = stoi ( string() + str[i-1] + str[i], nullptr, 10 );
        byteField.push_back ( value );
    }
    return byteField;
}
//-------------
vector<uint8_t> Converter::ASCIItoByte ( string text ) {
    vector<uint8_t> byteField;
    byteField.clear();
    for ( size_t i = 0; i < text.length(); i++ )
        byteField.push_back ( (uint8_t) text[i] );
    return byteField;
}
//-------------
string Converter::ByteToASCII ( const vector<uint8_t> & bytefield ) {
    string output;
    for ( size_t i = 0; i < bytefield.size(); i++ ) {
        output += static_cast<char> ( bytefield[i] );
    }
    return output;
}
//-------------
vector<string> Converter::byteToHexa ( const vector<uint8_t> & bytefield ) {
    vector<string> hexastring;
    for ( size_t i = 0; i < bytefield.size(); i++ )
        hexastring.push_back ( Converter::intToHexaString ( bytefield[i] ) );
    return hexastring;
}
//-------------
vector<uint8_t> Converter::byteXOR ( const vector<uint8_t> & field1, const vector<uint8_t> & field2 ) {
    vector<uint8_t> tmp;
    
    for ( size_t i = 0; i < field1.size(); i++ )
        tmp.push_back ( field1[i] ^ field2[i] );
    return tmp;
}

// int main () {

//     vector<uint8_t> str1 = Converter::hexaToByte ( "fc57c9cfdac7202694e8c2712d1f0e8e21b8c6f6d612d6c96847e42e6257b3f9ea98466cfc8761ed5e0308311deb15" );
//     vector<uint8_t> str2 = Converter::hexaToByte ( "a83fa0bcfaae5306f5c8b1144e6d6bfa01cca38ea23cf68707258b4a1b77c09185ed2a08dcf5048c3a237c5974983b" );
//     cout << Converter::ByteToASCII ( Converter::byteXOR ( str1, str2 ) ) << endl;
//     return 0;
// }