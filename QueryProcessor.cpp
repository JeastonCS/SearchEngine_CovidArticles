#include "QueryProcessor.h"
#include <iterator>
#include "stemmer/porter2_stemmer.h"
QueryProcessor::QueryProcessor(const IndexHandler & handler) {
    ih = handler;
}

vector<string> QueryProcessor::runQuery(string query) {
    // process/split query
    Porter2Stemmer::trim(query);
    Porter2Stemmer::stem(query);
//    stringstream ss(query);
//    istream_iterator<std::string> begin(ss);
//    istream_iterator<std::string> end;
//    vector<std::string> queryOrder(begin, end);


    vector<string> docsFinal;

//    cout << "DOCSFINAL: " << endl;
//    for (int i = 0; i < docsFinal.size(); i++) {
//        cout << docsFinal[i] << endl;
//    }
    ///////////  demo  /////////////
//    if(queryOrder.size() == 1) {
//        vector<string> docs= ih.getWordDocs(query);
//        for (string d: docs) {
//            docsFinal.emplace_back(d);
//        }
//    }
//    else {
//        // parse vector of query
//    }

    return ih.getWordDocs(query);
}

int QueryProcessor::findTFIDRStat(Document doc, string word) {
    // TODO find ways to get terms and which to use as parameters
    int tf = 1; // find term frequency / total words in doc
    int idf = 1; // log_e(total num of documents / number of documents with word)
    return tf * idf;
}

vector<Document> QueryProcessor::getUnion(vector<Document> lhs, vector<Document> rhs){
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

vector<Document> QueryProcessor::getIntersection(vector<Document> lhs, vector<Document> rhs) {
    vector<Document> list;

    for (Document x : rhs) {
        for (Document y: lhs) {
            if (x.title == y.title)
                list.push_back(x);
        }
    }

    return list;
}

vector<Document> QueryProcessor::getDifference(vector<Document> lhs, vector<Document> rhs) {
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

vector<Document> QueryProcessor::getAuthor(vector<Document> list) {
    return list;
}