#include <string>
#include "CNode.h"

using namespace std;

//Constructor
CNode::CNode(string& key_, int value_)
        :key(key_),
         value(value_),
         next(nullptr)
{
}

//Destructor
CNode::~CNode() = default;

//opportunity to change the key
void CNode::setKey(std::string &_key) {
    key = _key;
}

//opportunity to change next node in linked list
void CNode::setNext(CNode *_next) {
    next = _next;
}

//opportunity to set next value
void CNode::setValue(int val) {
    value = val;
}

//getter of next node
CNode* CNode::getNext() const {
    return next;
}

//getter of value
int CNode::getValue() const {
    return value;
}

std::string CNode::getKey() const {
    return key;
}
