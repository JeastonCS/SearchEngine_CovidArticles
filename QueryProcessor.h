#include "IndexHandler.h"
#include "Document.h"
#include <iostream>
#include <vector>
using namespace std;

class QueryProcessor{
private:
    IndexHandler ih;
    string word1;
    string word2;
    string name;

public:
//    QueryProcessor() = default;
    QueryProcessor(const IndexHandler &);

    vector<string> runQuery(string query, int numOfDocs);

    vector<Document> getUnion(vector<Document> lhs, vector<Document> rhs);
    vector<Document> getIntersection(vector<Document> lhs, vector<Document> rhs);
    vector<Document> getDifference(vector<Document> lhs, vector<Document> rhs);
    vector<Document> getAuthor(vector<Document> list);

    void stem(string& );
    vector<Document> stringToDoc(const vector<string>& strs);
    double findTFIDRStat(Document doc, string word, int querySize, int numOfDoc);
    void relevancySort(vector<Document>&);
};