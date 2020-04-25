#include "HashTable.h"
#include <unordered_map>
#include <iterator>
#include <sstream>
#include <fstream>
using namespace std;

HashTable::HashTable() {
    table = new HashNode* [tableSize];
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
                temp = curr;
                curr = curr->next;
            }
            delete temp;
        }
    }
    delete[] table;
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
        if (curr->value == v)
            return;
        while (curr->next != nullptr) {
            if (curr->value == v)
                return;
            curr = curr->next;
        }
        curr->next = new HashNode(k,v);
    } else {
        newAuthors++;
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
                temp = curr;
                curr = curr->next;
            }
            delete temp;
        }
    }
}

vector<string> HashTable::getAtKey(string k) {
    vector<string> docs;
    int h = hashFunc(k);
    if (table[h]!=nullptr) {
        HashNode* curr = table[h];
        while (curr != nullptr) {
            docs.push_back(curr->value);
            curr = curr->next;
        }
    }
    return docs;
}

void HashTable::writeToFile(const char* file) {
    ofstream out(file);
    for (int i = 0; i < tableSize; ++i) {
        if(table[i] != nullptr){
            HashNode* curr = table[i];
            out << curr->key << " ";
            while (curr != nullptr) {
                out << curr->value << " ";
                curr = curr->next;
            }
            out << '\n';
        }
    }
}

void HashTable::getFile(const char *file) {
    ifstream in(file);
    char* buf = new char[2048];
    while (in.getline(buf,2048,'\n')){
        string line = buf;
        stringstream ss(line);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> nameAndDocs(begin, end);

        string name = nameAndDocs[0];
        for (int i = 1; i < nameAndDocs.size(); ++i) {
            insert(name, nameAndDocs[i]);
        }
    }

    delete buf;
}