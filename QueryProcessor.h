#include "IndexHandler.h"
#include "Document.h"
#include <iostream>
#include <vector>
using namespace std;

class QueryProcessor{
private:
    IndexHandler ih;
    string name;
    int numOfDocs;

public:
//    QueryProcessor() = default;
    QueryProcessor(const IndexHandler &);

    vector<string> runQuery(string query, int numOfDocs);

    vector<Document> getUnion(vector<Document> lhs, vector<Document> rhs);
    vector<Document> getIntersection(vector<Document> lhs, vector<Document> rhs);
    vector<Document> getDifference(vector<Document> lhs, vector<Document> rhs);
    vector<Document> getAuthor(vector<Document> lhs,vector<Document> list);

    void stem(string& );
    vector<Document> stringToDoc(const string strs);
    double findTFIDRStat(int querySize, int numOfDoc, double termFreq);
    void relevancySort(vector<Document>&);
};