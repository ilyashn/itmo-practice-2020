
#ifndef COMPRESSIONALG_LZRESULT_H
#define COMPRESSIONALG_LZRESULT_H

#include <vector>
#include "Tuple.h"

/**
 * LZResult is used to store information encoded by LZ algorithm
 * it stores vector of tuples and sequence of literals
 */
class LZResult {
private:
    std::vector<Tuple> tuples;
    std::string literals;
public:
    //Constructor
    LZResult() = default;
    //Destructor
    ~LZResult();

    void addTuple(uint16_t len, uint16_t offset, uint8_t size);
    std::string getLiterals();
    void setLiterals(std::string &_literals);
    std::vector<Tuple> getTuples();
};
#endif //COMPRESSIONALG_LZRESULT_H
