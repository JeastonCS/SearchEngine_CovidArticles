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

    vector<Document> runQuery(string query, int numOfDocs);

    vector<Document> getUnion(vector<vector<Document>>& docs);
    vector<Document> getIntersection(vector<vector<Document>>& docs);
    vector<Document> getDifference(vector<Document>& terms, vector<vector<Document>>& docs);
    vector<Document> getAuthor(vector<Document>& terms, vector<vector<Document>>& docs);

    void stem(string& str);
    vector<Document> stringToDoc(const string &);
    double findTFIDRStat(int querySize, int numOfDoc, double termFreq);
    static void relevancySort(vector<Document>&);
};