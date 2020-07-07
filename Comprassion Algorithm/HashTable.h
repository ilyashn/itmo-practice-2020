#ifndef COMPRESSIONALG_HASHTABLE_H
#define COMPRESSIONALG_HASHTABLE_H

#define SHIFT 5
#define WINDOW_SIZE 65535

#include <string>
#include "CNode.h"
#include <vector>
using namespace std;
/**
 * HashTable is a data structure that used as a dictionary
 * It stores a linked list of CNode objects and size of hashtable
 */
class HashTable {
private:

    struct SMap{
        CNode *element = nullptr;
    };
    size_t size;
    SMap *hash_table;
    int calculateHash(const string& str) const;

public:
   HashTable(size_t _size);
   ~HashTable();
   void put(string& _key, size_t val);
   std::size_t get(const string& _key) const;
   std::size_t replace(string& _key, std::size_t val);
   std::vector<size_t> getValuesByKey(const string& _key, std::size_t index);

};
#endif //COMPRESSIONALG_HASHTABLE_H
