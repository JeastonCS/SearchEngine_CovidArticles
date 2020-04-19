#include<iostream>
#include<cstdlib>
#include<string>
#include <vector>
using namespace std;
const int tableSize = 10000;

class HashTable {
private:
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
};