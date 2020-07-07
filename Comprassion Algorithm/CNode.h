
#ifndef COMPRESSIONALG_CNODE_H
#define COMPRESSIONALG_CNODE_H

#include <string>
/**
 * CNode - is a node of hash chain
 * it stores a position in input stream and some symbols that used for hashing
 */
class CNode {
private:
    std::string key;
    int value;
    CNode *next;
public:
    CNode(std::string& _key, int _value);
    ~CNode();
    void setKey(std::string &_key);
    void setValue(int val);
    void setNext(CNode *_next);
    std::string getKey() const;
    int getValue() const;
    CNode *getNext() const;
};
#endif //COMPRESSIONALG_CNODE_H
