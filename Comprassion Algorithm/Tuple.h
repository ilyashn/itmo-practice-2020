
#ifndef COMPRESSIONALG_TUPLE_H
#define COMPRESSIONALG_TUPLE_H

#include <string>
#include <cstdint>
#include <cstring>

/**
 * Tuple used for LZ compression
 * it stores length of sequence, offset and size of occurrence
 */
class Tuple {
private:
    uint16_t len;
    uint16_t offset;
    uint8_t size;

public:
    // Constructor
    Tuple(uint16_t length, uint16_t _offset, uint8_t _size): len(length), offset(_offset), size(_size) {};
    //Destructor
    ~Tuple() = default;

    uint16_t getLength();
    uint16_t getOffset();
    uint8_t getSize();
};

#endif //COMPRESSIONALG_TUPLE_H
