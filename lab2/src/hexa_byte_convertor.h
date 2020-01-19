#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iomanip>
#include <vector>

using namespace std;

class Converter {
public:
    Converter () = delete;
    static vector<uint8_t> hexaToByte ( string str );
    static vector<uint8_t> byteStringToByteField ( string str );
    static vector<uint8_t> ASCIItoByte ( string text );
    static string ByteToASCII ( const vector<uint8_t> & bytefield );
    static vector<string> byteToHexa ( const vector<uint8_t> & bytefield );
    static vector<uint8_t> byteXOR ( const vector<uint8_t> & field1, const vector<uint8_t> & field2 );
private:
    static string intToHexaString ( uint8_t value ) {
        stringstream stream;
        stream << hex << value;
        return stream.str();
    }
};