//
// Created by sinha on 4/14/2020.
//
#include "Word.h"
using namespace std;


Word::Word() { }

// CHECK
Word::Word(const string& word, const string& doc) {
    this->word = word;
    docIDs.push_back(doc);
}

void Word::addDocID(const string& doc) {
    docIDs.push_back(doc);
}

vector<string>& Word::getDocIDs() {
    return docIDs;
}

bool Word::operator<(const Word& rhs) const {
    return word < rhs.word;
}

bool Word::operator>(const Word& rhs) const {
    return word > rhs.word;
}

bool Word::operator==(const Word& rhs) const {
    return word == rhs.word;
}