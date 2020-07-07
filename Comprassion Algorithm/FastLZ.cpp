#include <cstring>
#include "FastLZ.h"
#include "HashTable.h"
#include <cstdint>

using namespace std;

FastLZ::~FastLZ() {
    input.clear();
    output.clear();
}

//conversion of uint16_t to byte stream
void FastLZ::uint16ToBytes(uint16_t value, string &str) {
    unsigned char bytes[2];
    memcpy(bytes, (char*)&value, 2);
    for(char ch : bytes) {
        str.push_back((uint16_t)ch);
    }
}

//conversion of uint8_t to byte stream
void FastLZ::uint8ToBytes(uint8_t value, string &str) {
    unsigned char bytes[1];
    memcpy(bytes, (char*)&value, 1);
    for(char ch : bytes) {
        str.push_back((uint8_t)ch);
    }
}


/**
 * compression of data
 * it use a hash table to store a dictionary
 */
void FastLZ::compress(const char *inString, size_t size) {
    input.assign(inString, size);
    int occurrenceSize = FLZ_OCCURRENCE_LEN,
        windowSize = FLZ_MAX_OCCURRENCE,
        maxLen = FLZ_WINDOW_SIZE;
    HashTable hashTable(FLZ_MAX_SIZE);
    size_t i = 0,
    startPosition = 0;

    while(i < size - (occurrenceSize - 1)) {
        string subString = input.substr(i, occurrenceSize);
        std::size_t val = hashTable.replace(subString, i);

        if (val != -1 && i - val <= maxLen) {

            size_t index = i + occurrenceSize,
            prevStart = val,
            sz = occurrenceSize,
            prevIndex = val + occurrenceSize;

            while (input[index] == input[prevIndex]) {
                if (sz >= windowSize || index == size)
                    break;
                prevIndex++;
                index++;
                sz++;
            }

            for (std::size_t j = i + 1; j < i + sz; ++j) {
                subString = input.substr(j, occurrenceSize);
                hashTable.replace(subString, j);
            }
            convertToStr(i - startPosition, i - prevStart, sz, input.substr(startPosition, i - startPosition));
            i = i + sz;
            startPosition = i;
            continue;
        }
        i++;
    }

    if (startPosition < size ) {
        convertToStr(size - startPosition, 0, 0, input.substr(startPosition, size));
    }

}

//decompression of data
string FastLZ::decompress() {
    string decompressedStr;
    std::size_t startIndex = 0,
        index = 0;
    for (auto tuple : results.getTuples()) {
        decompressedStr.append( results.getLiterals().substr(startIndex, tuple.getLength()));
        if (tuple.getSize() <= tuple.getOffset())
            decompressedStr.append(decompressedStr.substr(tuple.getLength() - tuple.getOffset() + index, tuple.getSize()));
        else {
            std::size_t i = 0,
                j = decompressedStr.length() - tuple.getOffset();
            while (i < tuple.getSize()) {
                if (tuple.getOffset() > 0) {
                    if (i % tuple.getOffset() == 0)
                        j = decompressedStr.length() - tuple.getOffset();
                }

                decompressedStr += decompressedStr[j];
                ++j;
                ++i;
            }
        }
        startIndex += tuple.getLength();
        index = decompressedStr.size();
    }

    return decompressedStr;
}

//conversion of bytes stream to uint16_t
uint16_t FastLZ::bytesToUint16(string str) {
    unsigned char bytes[2];
    for (int i = 0; i < 2; i++) {
        bytes[i] = str[i];
    }
    uint16_t value;
    memcpy(&value, bytes, sizeof(uint16_t));
    return value;
}

//conversion of bytes stream to uint8_t
uint8_t FastLZ::bytesToUint8(string str) {
    char bytes[1];
    for (int i = 0; i < 1; i++) {
        bytes[i] = str[i];
    }
    uint8_t value;
    memcpy(&value, bytes, sizeof(uint8_t));
    return value;
}

//convert encoded data to string
string FastLZ::encodedToStr() {
    std::size_t substrStart = 0;
    string liters = results.getLiterals();
    string substring;
    string tempStr;
    for (auto tuple : results.getTuples()) {
        uint16ToBytes(tuple.getLength(), tempStr);
        uint16ToBytes(tuple.getOffset(), tempStr);
        uint8ToBytes(tuple.getSize(), tempStr);
        substring = liters.substr(substrStart, tuple.getLength());
        tempStr.append(substring);
        substrStart += tuple.getLength();
    }
    return tempStr;
}


//convert string to encoded data in LZResult
void FastLZ::strToEncoded(string& str) {
    std::size_t i = 0;
    string literals;

    while (i < str.size()) {
        uint16_t len = bytesToUint16(str.substr(i, 2));
        i += 2;
        uint16_t offset = bytesToUint16(str.substr(i, 2));
        i += 2;
        uint8_t size = bytesToUint8(str.substr(i, 1));
        i += 1;

        results.addTuple(len, offset, size);
        literals.append(str.substr(i, len));
        i += len;
    }
    results.setLiterals(literals);

}

//convert sequence of numbers and literals to byte stream
void FastLZ::convertToStr(uint16_t len, uint16_t offset, uint8_t size, string literals) {

    uint16ToBytes(len, output);
    uint16ToBytes(offset, output);
    uint8ToBytes(size, output);
    output += literals;
}

//get result of compression
string& FastLZ::getOut() {
    return output;
}
