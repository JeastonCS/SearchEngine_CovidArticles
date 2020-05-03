#include<iostream>
#include<cstdlib>
#include<string>
#include <vector>
#include "Document.h"
using namespace std;
const int tableSize = 100000;

class HashTable {
private:
    int newAuthors = 0;
    struct HashNode{
        HashNode(string k, string v){
            key = k; value = v;
            next = nullptr;
        }
        string key;
        string value;
        HashNode* next;
    };
    HashNode **table;
public:
    HashTable();
    ~HashTable();
    void insert(string k, string v);
    vector<string> getAtKey(string k);
    void remove(string k);
    void removeAll();
    int hashFunc(string k);
    void writeToFile(const char* file);
    void getFile(const char* file);
    int getNumOfAuthors();
};