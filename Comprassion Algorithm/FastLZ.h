
#ifndef COMPRESSIONALG_FASTLZ_H
#define COMPRESSIONALG_FASTLZ_H

#define FLZ_OCCURRENCE_LEN 3
#define FLZ_WINDOW_SIZE 65535
#define FLZ_MAX_OCCURRENCE 255
#define FLZ_MAX_SIZE 131072

#include "LZResult.h"
#include "LZ.h"

/**
 * FastLZ is "fast level" implementation of LZ77 algorithm
 * It stores results of compression in LZResult, input data and output data
 * it implements interface LZ
 */
class FastLZ : public LZ {
private:
    LZResult results;
    std::string input;
    std::string output;

    void convertToStr(uint16_t len, uint16_t offset, uint8_t size, std::string literals);
    uint16_t bytesToUint16(std::string str);
    uint8_t bytesToUint8(std::string str);
    void uint16ToBytes(uint16_t value, std::string &str);
    void uint8ToBytes(uint8_t value, std::string &str);

public:
    FastLZ() = default;
    ~FastLZ();
    virtual void compress(const char *inString, size_t size);
    virtual std::string decompress();
    virtual std::string encodedToStr();
    virtual void strToEncoded(std::string& str);
    virtual std::string& getOut();
};
#endif //COMPRESSIONALG_FASTLZ_H
