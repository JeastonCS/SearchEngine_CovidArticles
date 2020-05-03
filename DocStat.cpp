#include "DocStat.h"

DocStat::DocStat(){
    docID = "";
    tfidf = 0;
}
DocStat::DocStat(string s){
    docID = s;
    tfidf = 0;
}

DocStat::DocStat(string s, double stat) {
    docID = s;
    tfidf = stat;
}

DocStat::DocStat(const DocStat & rhs) {
    tfidf = rhs.tfidf;
    docID = rhs.docID;
}

bool DocStat::operator > (const DocStat& rhs){
    return tfidf > rhs.tfidf;
}

bool DocStat::operator < (const DocStat& rhs){
    return tfidf < rhs.tfidf;
}

bool DocStat::operator == (const DocStat& rhs){
    return tfidf == rhs.tfidf;
}