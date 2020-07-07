
#include "Tuple.h"
#include <cstring>
#include <cstdint>

// getters for class fields

uint16_t Tuple::getLength() {
    return len;
}

uint16_t Tuple::getOffset() {
    return offset;
}

uint8_t Tuple::getSize() {
    return size;
}