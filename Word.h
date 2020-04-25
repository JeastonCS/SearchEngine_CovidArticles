#include <iostream>
#include <vector>
using namespace std;
class Word{
private:
    string word;
    vector<string> docIDs;
    vector<double> termFrequencies;
    int numOfDocs;
public:
    Word(const string& word);
    Word(const string&, const string&, double);
    void addDocID(const string&);
    void addTermFrequency(double);
    vector<string>& getDocIDs();
    vector<double>& getTermFrequencies();

    // compares based on word, not docIDS
    bool operator>(const Word &) const;
    bool operator<(const Word &) const;
    bool operator==(const Word &) const;
    friend ostream & operator<<(ostream&, const Word &);

    void print();
};