
#ifndef COMPRESSIONALG_LZ_H
#define COMPRESSIONALG_LZ_H

#include <string>
using namespace std;
/**
 * interface of LZ algorithm
 */
class LZ {
public:
    virtual void compress(const char *inString, size_t size) = 0;
    virtual std::string encodedToStr() = 0;
    virtual std::string decompress() = 0;
    virtual void strToEncoded(std::string& str) = 0;
    virtual std::string& getOut() = 0;
};

#endif //COMPRESSIONALG_LZ_H
