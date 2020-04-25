#include "HashTable.h"
#include <iostream>
#include <vector>
using namespace std;

class AuthorIndex{
private:
    HashTable table;
public:
    void addAuthor(string word, string doc) {
        table.insert(word,doc);
    }
    vector<string> getAuthor(string name) {
        return table.getAtKey(name);
    }
    void writeToFile(const char * file) {
        table.writeToFile(file);
    }
    void populateFromFile(const char * file) {
        table.getFile(file);
    }
    void clear() {
        table.removeAll();
    }
};