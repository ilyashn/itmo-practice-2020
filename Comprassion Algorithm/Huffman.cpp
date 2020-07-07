#include "Huffman.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <iostream>

Huffman::Huffman(string &in) : decoded(in) {
}
//get decoded data from bitstring
void Huffman::getDecodedBuffer(string &bitString, vector<unsigned char> &buffer) {
    string bit;
    map<string, unsigned char> reverseCodes;
    for(map<unsigned char, string>::iterator i = codes.begin(); i != codes.end(); i++) {
        reverseCodes[i->second] = i->first;
    }
    for(int i = 0; i < bitString.size() - extraBits; i++) {
        bit += string(1, bitString[i]);
        if(reverseCodes.find(bit) != reverseCodes.end()) {
            buffer.push_back(reverseCodes[bit]);
            bit.clear();
        }
    }
}
//convert character to binary view
string Huffman::toBinary(unsigned char a)
{
    string output;
    while(a != 0) {
        char bit = a % 2 == 0 ? '0' : '1';
        output += bit;
        a /= 2;
    }
    if(output.size() < BYTE_SIZE) {
        int deficit = BYTE_SIZE - output.size();
        for(int i = 0; i < deficit; i++) {
            output += '0';
        }
    }
    reverse(output.begin(), output.end());
    return output;

}
//convert stream of bytes to int
int Huffman::strToInt(string str) {
    char bytes[4];
    for (int i = 0; i < 4; ++i) {
        bytes[i] = (int)str[i];
    }
    int value;
    memcpy(&value, bytes, 4);
    return value;
}
//convert to byte
void Huffman::writeToStr(string str, string &res) {
    unsigned char bit;
    for (int i = 0; i < BYTE_SIZE; ++i) {
        bit = (bit << 1) | (str[i] - '0');
    }
    res += bit;
}
//encode bitstring to byte
string Huffman::encode() {
    string bits;
    string res;
    for (int i = 0; i < decoded.size(); ++i) {
        bits += codes[decoded[i]];
        while (bits.size() >= BYTE_SIZE) {
            writeToStr(bits.substr(0, BYTE_SIZE), res);
            bits.erase(0, BYTE_SIZE);
        }
    }
    if (bits.size() > 0 && bits.size() != BYTE_SIZE) {
        extraBits = BYTE_SIZE - bits.size();
        bits.append(extraBits, '0');
        writeToStr(bits.substr(0, BYTE_SIZE), res);
    }
    res[res.size()] = '\0';
    return res;
}
//Huffman compression algorithm
void Huffman::compressString() {
    int sz = decoded.size();
    extraBits = 0;
    map<unsigned char, std::size_t> frequencies;
    for (int i = 0; i < sz; ++i) {
        frequencies[decoded[i]]++;
    }
    buildHuffmanTree(frequencies);
    traverseHuffmanTree(root, "", '2');
    string res = encode();

    putHeader();
    encoded.append(res);
}
//write header to encoded string
void Huffman::putHeader() {
    intToCharArray(extraBits);
    int sz = codes.size();

    intToCharArray(sz);
    for(map<unsigned char, string>::iterator i = codes.begin(); i != codes.end(); i++) {
        encoded += i->first;
        int len = i->second.size();
        intToCharArray(len);
        encoded += i->second;
    }
}
//conversion int to bytes stream
void Huffman::intToCharArray(int &value) {
    char bytes[4];
    memcpy(bytes, (char*)&value, 4);
    for(char ch : bytes) {
        encoded.push_back((int)ch);
    }
}

//getter for encoded string
string Huffman::getEncodedString() {
    return encoded;
}
//building huffman binary tree
void Huffman::buildHuffmanTree(map<unsigned char, size_t> &frequencies) {
    priority_queue<Tree *, vector<Tree *>, TreeComparator> priorityQueue;
    for (map<unsigned char, size_t>::iterator i = frequencies.begin(); i != frequencies.end(); i++) {
        Tree *node = new Tree();
        node->frequency = i->second;
        node->character = i->first;
        priorityQueue.push(node);
    }

    while (priorityQueue.size() > 1) {
        Tree *a, *b;
        a = priorityQueue.top();
        priorityQueue.pop();

        b = priorityQueue.top();
        priorityQueue.pop();
        Tree *c = new Tree();
        c->frequency = a->frequency + b->frequency;
        c->left = a;
        c->right = b;
        priorityQueue.push(c);
    }
    root = priorityQueue.top();

}

//traverse of huffman tree
void Huffman::traverseHuffmanTree(Tree *parent, string previousBits, char newBit){
    if (newBit != '2')
        previousBits += newBit;

    if (parent->right == NULL && parent->left == NULL) {
        codes[parent->character] = previousBits;
    }
    if (parent->right != NULL) {
        traverseHuffmanTree(parent->right, previousBits, '1');
    }

    if (parent->left != NULL) {
        traverseHuffmanTree(parent->left, previousBits, '0');
    }
}

//Huffman decompression
void Huffman::decompressString(string &output) {
    encoded = output;
    extraBits = 0;
    int shift = readHeader();
    string fileBitString = getStringFromBuffer(shift);
    vector<unsigned char> outputVector;
    getDecodedBuffer(fileBitString, outputVector);
    unsigned char * outputBuffer = outputVector.data();
    decoded.append((char*)outputBuffer, outputVector.size());
}

//getter for decoded string
string Huffman::getDecodedString(){
    return decoded;
}

//get data without header and convert it to binary view
string Huffman::getStringFromBuffer(std::size_t shift) {
    string bitString;
    for(int i = shift; i < encoded.size(); i++) {
        bitString += toBinary(encoded[i]);
    }
    return bitString;

}
//reading a Header of encoded data
int Huffman::readHeader() {
    int j = 0;
    extraBits = strToInt(encoded.substr(j, 4));
    j += 4;

    int size = strToInt(encoded.substr(j, 4));

    j += 4;

    for(int i = 0; i < size; i++) {
        unsigned char key = encoded[j];
        j++;
        int len = strToInt(encoded.substr(j, 4));
        j+=4;

        string values = encoded.substr(j, len);

        j += len;

        codes[key] = values;
    }
    return j;
}

