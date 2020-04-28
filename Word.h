#include "Document.h"

#include <iostream>
#include <vector>
using namespace std;
class Word{
private:
    string word;
    vector<Document> docIDs;
    vector<double> termFrequencies;
    int numOfDocs;
public:
    Word(const string&);
    Word(const string&, const Document&, double);
    void addDoc(const Document&);
    void addTermFrequency(double);
    vector<Document>& getDocs();
    vector<double>& getTermFrequencies();

    // compares based on word, not docIDS
    bool operator>(const Word &) const;
    bool operator<(const Word &) const;
    bool operator==(const Word &) const;
    friend ostream & operator<<(ostream&, const Word &);

    void print();
};