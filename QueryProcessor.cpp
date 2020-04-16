#include "QueryProcessor.h"
#include "Word.h"
#include <iterator>
#include <sstream>
void QueryProcessor::runQuery(string query) {
    // process query
    stringstream ss(query);
    istream_iterator<std::string> begin(ss);
    istream_iterator<std::string> end;
    vector<std::string> queryOrder(begin, end);

    ///////////  demo  /////////////
    // queryOrder is size 1
    // TODO research functions in index handler
}

int QueryProcessor::findTFIDRStat(Document doc, string word) {
    // TODO find ways to get terms and which to use as parameters
    int tf = 1; // find term frequency / total words in doc
    int idf = 1; // log_e(total num of documents / number of documents with word)
    return tf * idf;
}

vector<Document>& QueryProcessor::getUnion(vector<Document> lhs, vector<Document> rhs){
    vector<Document> list = lhs;

    for (Document x : rhs) {
        bool found = false;
        for (Document y: list) {
            if(x.title != y.title) {
                found = true;
                break;
            }
        }
        if(!found)
            list.push_back(x);
    }

    return list;
}

vector<Document>& QueryProcessor::getIntersection(vector<Document> lhs, vector<Document> rhs) {
    vector<Document> list;

    for (Document x : rhs) {
        for (Document y: lhs) {
            if (x.title == y.title)
                list.push_back(x);
        }
    }

    return list;
}

vector<Document>& QueryProcessor::getDifference(vector<Document> lhs, vector<Document> rhs) {
    vector<Document> list = lhs;

    for (Document x : rhs) {
        bool notFound = false;
        for (Document y: list) {
            if(x.title == y.title) {
                notFound = true;
                break;
            }
        }
        if(!notFound)
            list.push_back(x);
    }

    return list;
}

vector<Document>& QueryProcessor::getAuthor(vector<Document> list) {
    return list;
}