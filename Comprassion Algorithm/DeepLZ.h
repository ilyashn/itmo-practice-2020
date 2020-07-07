#ifndef COMPRESSIONALG_DEEPLZ_H
#define COMPRESSIONALG_DEEPLZ_H

#define DLZ_WINDOW_SIZE 65535
#define DLZ_MAX_OCCURRENCE 255
#define DLZ_TABLE_SIZE 31072
#define DLZ_OCCURRENCE_LEN 5

#include <string>
#include "LZResult.h"
#include "LZ.h"

/**
 * DeepLZ is "deep level" implementation of LZ77 algorithm
 * It stores results of compression in LZResult, input data and output data
 * it implements interface LZ
 */
class DeepLZ : public LZ {
private:
    std::string input;
    std::string output;
    LZResult results;

    uint16_t bytesToUint16(std::string str);
    uint8_t bytesToUint8(std::string str);
    void uint16ToBytes(uint16_t value, std::string &str);
    void uint8ToBytes(uint8_t value, std::string &str);
    void convertToStr(uint16_t len, uint16_t offset, uint8_t size, std::string literals);

public:
    //Constructor
    DeepLZ() = default;
    //Destructor
    ~DeepLZ();
    virtual void compress(const char *inString, size_t size);
    virtual std::string encodedToStr();
    virtual std::string decompress();
    virtual void strToEncoded(std::string& str);
    virtual std::string& getOut();
};


#endif //COMPRESSIONALG_DEEPLZ_H
