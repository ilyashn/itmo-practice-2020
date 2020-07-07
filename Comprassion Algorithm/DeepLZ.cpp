#include "DeepLZ.h"
#include "HashTable.h"
#include <cstdint>
#include <cstring>

using namespace std;

//get result of compression
string& DeepLZ::getOut() {
    return output;
}
//convert sequence of numbers and literals to byte stream
void DeepLZ::convertToStr(uint16_t len, uint16_t offset, uint8_t size, string literals) {
    uint16ToBytes(len, output);
    uint16ToBytes(offset, output);
    uint8ToBytes(size, output);
    output += literals;
}
//conversion of bytes stream to uint16_t
uint16_t DeepLZ::bytesToUint16(string str) {
    char bytes[2];
    for (int i = 0; i < 2; i++) {
        bytes[i] = (uint16_t)str[i];
    }
    uint16_t value;
    memcpy(&value, bytes, 2);
    return value;
}

//conversion of bytes stream to uint8_t
uint8_t DeepLZ::bytesToUint8(string str) {
    char bytes[1];
    for (int i = 0; i < 1; i++) {
        bytes[i] = (uint8_t)str[i];
    }
    uint8_t value;
    memcpy(&value, bytes, 1);
    return value;
}

//conversion of uint16_t to byte stream
void DeepLZ::uint16ToBytes(uint16_t value, string &str) {
    char bytes[2];
    memcpy(bytes, (char*)&value, 2);
    for(char ch : bytes) {
        str.push_back((uint16_t)ch);
    }
}

//conversion of uint8_t to byte stream
void DeepLZ::uint8ToBytes(uint8_t value, string &str) {
    char bytes[1];
    memcpy(bytes, (char *) &value, 1);
    for (char ch : bytes) {
        str.push_back((uint8_t) ch);
    }
}

/**
 * compression of data
 * it use a hash table to store a dictionary
 */
 void DeepLZ::compress(const char *inString, std::size_t size) {
    input.assign(inString, size);
    std::size_t occurrenceSize = DLZ_OCCURRENCE_LEN,
            windowSize = DLZ_MAX_OCCURRENCE,
            maxLen = DLZ_WINDOW_SIZE;
    HashTable hashTable(DLZ_TABLE_SIZE);
    std::size_t i = 0,
            startPosition = 0;

    while (i < size - (occurrenceSize - 1)) {

        string subString = input.substr(i, occurrenceSize);
        std::vector<std::size_t> values = hashTable.getValuesByKey(subString, i);

        if (values.size() == 0)
            hashTable.put(subString, i);
        else {

            std::size_t maxSize = 0, maxIndex = 0;
            for (int j = 0; j < values.size(); j++) {
                if (i - values[j] <= maxLen) {
                    std::size_t sz = occurrenceSize,
                            index = i + occurrenceSize,
                            prevIndex = values[j] + occurrenceSize;

                    while (input[prevIndex] == input[index]) {
                        if (sz >= windowSize || index == size)
                            break;
                        prevIndex++;
                        index++;
                        sz++;
                    }

                    if (maxSize < sz) {
                        maxSize = sz;
                        maxIndex = values[j];
                    }
                }
            }
            for (std::size_t j = i + 1; j < i + maxSize; ++j) {
                subString = input.substr(j, occurrenceSize);
                hashTable.put(subString, j);
            }

            convertToStr(i - startPosition, i - maxIndex, maxSize, input.substr(startPosition, i - startPosition));
            i = i + maxSize;
            startPosition = i;
            continue;
        }
        i++;
    }
    if (startPosition < size) {
        convertToStr(size - startPosition, 0, 0, input.substr(startPosition, size));
    }
}

//convert string to encoded data in LZResult
void DeepLZ::strToEncoded(string& str) {
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
//decompression of data
string DeepLZ::decompress() {
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
//convert encoded data to string
string DeepLZ::encodedToStr() {
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
//Destructor
DeepLZ::~DeepLZ() {
    input.clear();
    output.clear();
}