//
// Created by Jack Easton on 4/22/2020.
//

#include "DocumentWord.h"

DocumentWord::DocumentWord(string word) {
    this->word = word;
    timesInDoc = 1;
}

bool DocumentWord::operator==(const DocumentWord &rhs) const {
    return word == rhs.word;
}

ostream &operator<<(ostream &os, const DocumentWord &docWord) {
    os << docWord.word << " (" << docWord.termFreq << ")";

    return os;
}

void DocumentWord::initTermFreq(int docWordCount) {
    termFreq = (double) timesInDoc / docWordCount;
}
