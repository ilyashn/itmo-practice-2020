#ifndef UNTITLED_HUFFMAN_H
#define UNTITLED_HUFFMAN_H

#define BYTE_SIZE 8

#include <string>
#include <map>
#include <vector>

using namespace std;

/**
 * struct that implements a binary tree
 * it stores a character, amount of occurrence of this character, and pointers to the left and right children
 */
struct Tree
{
    int frequency;
    unsigned char character;
    Tree *left = nullptr;
    Tree *right = nullptr;
};

/**
 * Comparator for comparison of trees by frequencies
 */
class TreeComparator
{
public:
    bool operator()(Tree *a, Tree *b)
    {
        return a->frequency > b->frequency;
    }
};

/**
 * Huffman coding algorithm
 * it stores extraBits that needed to store full last byte, encoded and decoded string codes of literals(bytes) and root element of binary tree
 */
class Huffman {
private:
    int extraBits = 0;
    string decoded;
    string encoded;
    map<unsigned char,string> codes;
    Tree* root = nullptr;

    void buildHuffmanTree(map<unsigned char, std::size_t>&);
    void traverseHuffmanTree(Tree *parent, string prev, char toAppend);
    void putHeader();
    void intToCharArray(int &value);
    string toBinary(unsigned  char a);
    string encode();
    void writeToStr(string str, string &res);
    void getDecodedBuffer(string &bitString, vector<unsigned char> &buffer);
    string getStringFromBuffer(std::size_t shift);
    int strToInt(string str);
    int readHeader();

public:
    //Constructors
    Huffman(string &in);
    Huffman() = default;
    void compressString();
    string getEncodedString();
    void decompressString(string &output);
    string getDecodedString();
};


#endif //UNTITLED_HUFFMAN_H
