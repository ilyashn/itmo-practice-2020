
#include "LZResult.h"

//adding a new Tuple in a vecctor
void LZResult::addTuple(uint16_t len, uint16_t offset, uint8_t size) {
    tuples.emplace_back(len, offset, size);
}

//getter for vector of tuples
std::vector<Tuple> LZResult::getTuples() {
    return tuples;
}

//opportunity to set sequence of literals
void LZResult::setLiterals(std::string &_literals) {
    literals = _literals;
}

//getter for sequence of literals
std::string LZResult::getLiterals() {
    return literals;
}

LZResult::~LZResult() {
    literals.clear();
    tuples.clear();
}