//
// Created by sinha on 4/14/2020.
//
#include "Word.h"
using namespace std;


Word::Word(const string& word) {
    this->word = word;
    numOfDocs = 0;
}

// CHECK
Word::Word(const string& word, const string& doc, double termFrequency) {
    this->word = word;
    docIDs.push_back(doc);
    termFrequencies.push_back(termFrequency);
    numOfDocs = 1;
}

void Word::addDocID(const string& doc) {
    docIDs.push_back(doc);
    numOfDocs++;
}

void Word::addTermFrequency(int frequency) {
    termFrequencies.push_back(frequency);
}

vector<string>& Word::getDocIDs() {
    return docIDs;
}

vector<double>& Word::getTermFrequencies() {
    return termFrequencies;
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

ostream &operator<<(ostream &os, const Word &rhs) {
    os << rhs.word << " ";

    for (int i = 0; i < rhs.docIDs.size(); i++)
        os << rhs.docIDs[i] << " " << rhs.termFrequencies[i] << " ";

    os << flush;
    return os;
}

void Word::print() {
    cout << *this;
}
