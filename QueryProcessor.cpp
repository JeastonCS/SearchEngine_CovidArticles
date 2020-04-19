#include "QueryProcessor.h"
#include <iterator>
#include "stemmer/porter2_stemmer.h"
QueryProcessor::QueryProcessor(const IndexHandler & handler) {
    ih = handler;
}

void QueryProcessor::stem(string & str) {
    Porter2Stemmer::trim(str);
    Porter2Stemmer::stem(str);
}

vector<Document> QueryProcessor::stringToDoc(const vector<string> &strs) {
    vector<Document> docs;
    for (string s:strs) {
        Document strToDoc = * new Document(s);
        docs.push_back(strToDoc);
    }
    return docs;
}

vector<string> QueryProcessor::runQuery(string query, int numOfDocs) {
    // process/split query
    stringstream ss(query);
//    istream_iterator<string> begin(ss);
//    istream_iterator<string> end;
//    vector<string> queryOrder(begin, end);


    vector<string> docsFinal;
    /////////  demo  /////////////
    stem(query);
    docsFinal = ih.getWordDocs(query);
    //////////////////////////////

    //TODO get Author Last Name or first and take union
//    vector<Document> currentList;
//    string param1 = "";
//    string param2 = "";
//
//    while (!queryOrder.empty()){
//        string curr = queryOrder[queryOrder.size() - 1];
//
//        if(curr == "AUTHOR"){
//           currentList = stringToDoc(ih.getAuthorDocs(param1));
//           param1 = "";
//        }
//
//        else if(curr == "AND"){
//            vector<Document> d1;
//            if(!currentList.empty())
//                d1 = currentList;
//            else
//                d1 = stringToDoc(ih.getWordDocs(param1));
//            vector<Document> d2 = stringToDoc(ih.getWordDocs(param2));
//            currentList = getIntersection(d2,d1);
//
//            param1 = ""; param2 = "";
//        }
//
//        else if(curr == "OR"){
//            vector<Document> d1;
//            if(!currentList.empty())
//                d1 = currentList;
//            else
//                d1 = stringToDoc(ih.getWordDocs(param1));
//            vector<Document> d2 = stringToDoc(ih.getWordDocs(param2));
//            currentList = getUnion(d2,d1);
//
//            param1 = ""; param2 = "";
//        }
//
//        else if(curr == "NOT"){
//            vector<Document> d1;
//            if(!currentList.empty())
//                d1 = currentList;
//            else
//                d1 = stringToDoc(ih.getWordDocs(param1));
//            string nextParam = queryOrder[queryOrder.size() - 2];
//            stem(nextParam);
//            vector<Document> d2 = stringToDoc(ih.getWordDocs(nextParam));
//            currentList = getDifference(d2,d1);
//
//            param1 = ""; param2 = "";
//            queryOrder.pop_back();
//        }
//
//        else {
//            if(param1 == "") {
//                param1 = curr;
//                stem(param1);
//            }
//            else {
//                param2 = curr;
//                stem(param2);
//            }
//        }
//
//        if(param1 != "" && queryOrder.size() == 1)
//            currentList = stringToDoc(ih.getWordDocs(param1));
//
//        queryOrder.pop_back();
//    }
//    //TODO convert back to string for now
//    //TODO need to complete TF-IDR stat for sort
//    for (Document x: currentList)
//        docsFinal.push_back(x.title);
    return docsFinal;
}

double QueryProcessor::findTFIDRStat(Document doc, string word, int querySize ,int numOfDocs) {
    // TODO find ways to get terms and which to use as parameters
    double tf = 1; // find term frequency / total words in doc
    double idf = numOfDocs / querySize; // log_e(total num of documents / number of documents with word)
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
        for (Document y : lhs) {
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

void QueryProcessor::relevancySort(vector<Document> & list) {
    // use insertion sort
    //TODO need compare operators in Document class
    int size = list.size();

    int i, j;
    Document key;
    for (i = 1; i < size; i++) {
        key = list[i];
        j = i - 1;
        while (j >= 0 && list[j] > key) {
            list[j + 1] = list[j];
            j = j - 1;
        }
        list[j + 1] = key;
    }
}

vector<Document> QueryProcessor::getAuthor(vector<Document> list) {
    return list;
}