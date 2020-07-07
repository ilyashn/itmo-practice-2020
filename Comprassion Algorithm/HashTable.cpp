#include "CNode.h"
#include "HashTable.h"

//our hash function
int HashTable::calculateHash(const string& str) const {
    long long temp = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        temp = (temp << SHIFT) ^ (str[i]);
        temp %= size;
    }
    return temp;
}

//Constructor
HashTable::HashTable(std::size_t _size)
    :size(_size),
    hash_table(new SMap[_size])
{}

//Destructor
HashTable::~HashTable() {
    delete [] hash_table;
}
//adding a new pair(key, value) to the head one of linked list
void HashTable::put(string &_key, std::size_t val) {
    int index = calculateHash(_key);
    CNode *temp = hash_table[index].element;

    if (temp == nullptr){
        hash_table[index].element = new CNode(_key, val);
        return;
    }
    CNode *tmp = new CNode(_key, val);
    tmp->setNext(temp);
    hash_table[index].element = tmp;
}

//get value by key
size_t HashTable::get(const string &_key) const {
    int index = calculateHash(_key);
    CNode *temp = hash_table[index].element;

    while (temp != nullptr){
        if (temp->getKey() == _key)
            return temp->getValue();
        temp = temp->getNext();
    }
    return -1;
}

//this function replace value if key already exists and return old value, else it just put a new pair(key, value) and return -1
size_t HashTable::replace(string &_key, size_t val) {
    int index = calculateHash(_key);
    CNode *temp = hash_table[index].element;

    if (temp == nullptr){
        hash_table[index].element = new CNode(_key, val);
        return - 1;
    }

    while (temp->getNext() && temp->getKey() != _key ){
        temp = temp->getNext();
    }

    if (temp->getKey() == _key) {
        size_t value = temp->getValue();
        temp->setValue(val);
        return value;
    }

    if (temp->getNext() == nullptr ) {
        CNode *tmp = new CNode(_key, val);
        tmp->setNext(hash_table[index].element);
        hash_table[index].element = tmp;

        return -1;
    }
    return -1;
}

//this function get all values by given key, also it deletes old values from hashtable
std::vector<size_t> HashTable::getValuesByKey(const string &_key, std::size_t index) {
    std::vector<std::size_t> values;
    int id = calculateHash(_key);
    CNode *temp = hash_table[id].element;
    CNode *prev = nullptr;
    CNode *tmp;

    while (temp != nullptr){
        if (index - temp->getValue() > WINDOW_SIZE) {
            if (prev) {
                prev->setNext(temp->getNext());
                tmp = prev->getNext();
            }
            else {
                hash_table[id].element = temp->getNext();
                tmp = hash_table[id].element;
            }
            delete temp;
            temp = tmp;
        }

        if (!temp)
            break;

        if (temp->getKey() == _key && index - temp->getValue() <= WINDOW_SIZE)
            values.push_back(temp->getValue());

        prev = temp;
        temp = temp->getNext();
    }

    return values;
}
