#include <iostream>
#include <vector>
using namespace std;
class Word{
private:
    string word;
    vector<string> docIDs;
    int numOfDocs;
public:
    Word(const string& word);
    Word(const string&, const string&);
    void addDocID(const string&);
    vector<string>& getDocIDs();

    // compares based on word, not docIDS
    bool operator>(const Word &) const;
    bool operator<(const Word &) const;
    bool operator==(const Word &) const;
};