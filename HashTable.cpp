#include "HashTable.h"
#include <unordered_map>
using namespace std;

HashTable::HashTable() {
    table = new HashNode *[tableSize];
    for (int i = 0; i < tableSize; i++) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; i++) {
        if (table[i] != nullptr){
            HashNode* temp = table[i];
            HashNode* curr = temp->next;
            while (curr != nullptr) {
                delete temp;
                temp = curr->next;
                curr = curr->next;
            }
            delete temp;
        }
        delete[] table;
    }
}

// modify and use different library
int HashTable::hashFunc(string k) {
    hash<string> hasher;
    return hasher(k) % tableSize;
}


void HashTable::insert(string k, string v) {
    int h = hashFunc(k);
    if (table[h] != nullptr) {
        HashNode* curr = table[h];
        while (curr->next != nullptr)
            curr = curr->next;
        curr->next = new HashNode(k,v);
    } else {
        table[h] = new HashNode(k, v);
    }
}

void HashTable::remove(string k) {
    int h = hashFunc(k);
    if (table[h] != nullptr) {
        HashNode* temp = table[h];
        HashNode* curr = temp->next;
        while (curr != nullptr) {
            delete temp;
            temp = curr->next;
            curr = curr->next;
        }
        delete temp;
    }
}

void HashTable::removeAll() {
    for (int i = 0; i < tableSize; i++) {
        if (table[i] != nullptr){
            HashNode* temp = table[i];
            HashNode* curr = temp->next;
            while (curr != nullptr) {
                delete temp;
                temp = curr->next;
                curr = curr->next;
            }
            delete temp;
        }
    }
}

vector<string>& HashTable::getAtKey(string k) {
    vector<string> docs;
    int h = hashFunc(k);
    if (table[h]!=nullptr) {
        HashNode* curr = table[h];
        while (curr->next != nullptr) {
            docs.push_back(curr->value);
            curr = curr->next;
        }
    }
    return docs;
}