#include "IndexHandler.h"
#include "DocStat.h"
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
    vector<string> stopWords;
private:
    vector<DocStat> getUnion(vector<vector<DocStat>>& docs);
    vector<DocStat> getIntersection(vector<vector<DocStat>>& docs);
    vector<DocStat> getDifference(vector<DocStat>& terms, vector<vector<DocStat>>& docs);
    vector<DocStat> getAuthor(vector<DocStat>& terms, vector<vector<DocStat>>& docs);

    void populateStopWords();
    void stem(string& str);
    vector<DocStat> stringToDoc(const string &);
    double findTFIDRStat(int querySize, int numOfDoc, double termFreq);
    static void relevancySort(vector<DocStat>&);
};