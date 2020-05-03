#include "HashTable.h"
#include <iostream>
#include <vector>
using namespace std;

class AuthorIndex{
private:
    HashTable table;
public:
    AuthorIndex &operator=(const AuthorIndex &rhs) {
        table = rhs.table;
        return *this;
    }
    void addAuthor(string word, const string &doc) {
        table.insert(word,doc);
    }
    vector<string> getAuthor(string name) {
        return table.getAtKey(name);
    }
    void writeToFile(const char * file) {
        table.writeToFile(file);
    }
    void populateFromFile(char * file) {
        table.getFile(file);
    }
    void clear() {
        table.removeAll();
    }
    int getNumUniqueAuthors() {
        return table.getNumOfAuthors();
    }
};