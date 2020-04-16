#include "IndexHandler.h"
#include <iostream>
#include <vector>
using namespace std;
struct Document{
    string title;
    string author;
    string publication;
    double tfStat;
    Document(string t) {title = t;}
};

class QueryProcessor{
private:
    IndexHandler ih;
    string word1;
    string word2;
    string name;

public:
    QueryProcessor();
    QueryProcessor(const IndexHandler &);

    vector<Document>& runQuery(string query);

    vector<Document>& getUnion(vector<Document> lhs, vector<Document> rhs);
    vector<Document>& getIntersection(vector<Document> lhs, vector<Document> rhs);
    vector<Document>& getDifference(vector<Document> lhs, vector<Document> rhs);
    vector<Document>& getAuthor(vector<Document> list);

    int findTFIDRStat(Document doc, string word);
    void relevancySort(vector<Document>&);
};