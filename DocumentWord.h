//
// Created by Jack Easton on 4/22/2020.
//

#ifndef SEARCHENGINE_DOCUMENTWORD_H
#define SEARCHENGINE_DOCUMENTWORD_H

#include <string>
#include <fstream>
using namespace std;

class DocumentWord {
private:
    string word;
    int timesInDoc;
    double termFreq;
public:
    DocumentWord(string);

    bool operator==(const DocumentWord &) const;
    friend ostream & operator<<(ostream &, const DocumentWord &);

    void incrementTimesInDoc() { timesInDoc++; }
    void initTermFreq(int);

    string getWord() { return word; }
    double getTermFreq() { return termFreq; }
};


#endif //SEARCHENGINE_DOCUMENTWORD_H
