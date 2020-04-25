#include "QueryProcessor.h"
#include <iterator>
#include <math.h>
#include <algorithm>
#include "stemmer/porter2_stemmer.h"
QueryProcessor::QueryProcessor(const IndexHandler & handler) {
    ih = handler;
}

void QueryProcessor::stem(string & str) {
    Porter2Stemmer::trim(str);
    Porter2Stemmer::stem(str);
}

vector<Document> QueryProcessor::stringToDoc(const string word) {
    vector<string> strs = ih.getWordDocs(word);
    vector<double> freq = ih.getWordFreq(word);
    vector<Document> docs;
    int i = 0;
    for (string s:strs) {
        Document strToDoc = * new Document(s);
        strToDoc.tfStat = findTFIDRStat(strs.size(),numOfDocs,freq[i]);
        docs.push_back(strToDoc);
        i++;
    }
    return docs;
}

vector<string> QueryProcessor::runQuery(string query, int numDocs) {
    numOfDocs = numDocs;

    // process/split query
    stringstream ss(query);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> queryOrder(begin, end);

    // temporarily output as string
    //TODO Need to load Title, Description, Authors, and Publication Date Per result
    vector<string> docsFinal;

    vector<Document> currentList;
    string param1 = "";
    string param2 = "";

    while (!queryOrder.empty()){
        string curr = queryOrder[queryOrder.size() - 1];

        if(curr == "AUTHOR"){
            // get Authors and convert to Documents
            vector<string> authors = ih.getAuthorDocs(param1);
            vector<Document> d1;
            for (string s: authors)
                d1.push_back(*new Document(s));

            // get Query word
            string nextParam = queryOrder[queryOrder.size() - 2];
            vector<Document> d2 = stringToDoc(nextParam);

            // run AuthorUnion and sort
            currentList = getAuthor(d2,d1);
            param1 = ""; param2 = "";
            queryOrder.pop_back();
        }

        else if(curr == "AND"){
            stem(param1); stem(param2);
            vector<Document> d1;
            if(!currentList.empty())
                d1 = currentList;
            else
                d1 = stringToDoc(param1);
            vector<Document> d2 = stringToDoc(param2);
            currentList = getIntersection(d2,d1);

            param1 = ""; param2 = "";
        }

        else if(curr == "OR"){
            stem(param1); stem(param2);
            vector<Document> d1;
            if(!currentList.empty())
                d1 = currentList;
            else
                d1 = stringToDoc(param1);
            vector<Document> d2 = stringToDoc(param2);
            currentList = getUnion(d2,d1);

            param1 = ""; param2 = "";
        }

        else if(curr == "NOT"){
            stem(param1); stem(param2);
            vector<Document> d1;
            if(!currentList.empty())
                d1 = currentList;
            else
                d1 = stringToDoc(param1);
            string nextParam = queryOrder[queryOrder.size() - 2];
            stem(nextParam);
            vector<Document> d2 = stringToDoc(nextParam);
            currentList = getDifference(d2,d1);

            param1 = ""; param2 = "";
            queryOrder.pop_back();
        }

        else {
            if(param1 == "")
                param1 = curr;
            else
                param2 = curr;
        }

        // if query is only one word
        if(param1 != "" && queryOrder.size() == 1) {
            stem(param1);
            currentList = stringToDoc(param1);
            relevancySort(currentList);
        }

        queryOrder.pop_back();
    }
    //TODO convert back to string for now
    for (Document x: currentList)
        docsFinal.push_back(x.docID);
    return docsFinal;
}

double QueryProcessor::findTFIDRStat(int querySize,int numOfDocs, double termFreq) {
    double tf = termFreq; // find term frequency in doc / total words in doc -> done in DocumentWord
    double idf = log((numOfDocs*1.0) / (querySize) + 1); // log_e(total num of documents / number of documents with word + 1)
    return tf * idf;
}

vector<Document> QueryProcessor::getUnion(vector<Document> lhs, vector<Document> rhs){
    vector<Document> list = lhs;

    for (Document x : rhs) {
        bool found = false;
        for (Document y: list) {
            if(x.docID == y.docID) {
                y.tfStat = y.tfStat + x.tfStat; // change to make more precise
            }
            if(x.docID != y.docID) {
                found = true;
                break;
            }
        }
        if(!found)
            list.push_back(x);
    }
    relevancySort(list);
    return list;
}

vector<Document> QueryProcessor::getIntersection(vector<Document> lhs, vector<Document> rhs) {
    vector<Document> list;

    for (Document x : rhs) {
        for (Document y : lhs) {
            if (x.docID == y.docID) {
                x.tfStat = x.tfStat * y.tfStat; // edit to make more precise
                list.push_back(x);
            }
        }
    }

    relevancySort(list);
    return list;
}

// lhs NOT rhs
vector<Document> QueryProcessor::getDifference(vector<Document> lhs, vector<Document> rhs) {
    vector<Document> list;

    for (Document x : lhs) {
        bool found = false;
        for (Document y: rhs) {
            if(x.docID == y.docID) {
                found = true;
                break;
            }
        }
        if(!found) {
            list.push_back(x);
        }
    }

    relevancySort(list);
    return list;
}


vector<Document> QueryProcessor::getAuthor(vector<Document> lhs, vector<Document> aList) {
    vector<Document> list;
    cout << aList.size() <<endl;
    for (Document x : aList) {
        for (Document y : lhs) {
            if (x.docID == y.docID)
                list.push_back(x);
        }
    }

    relevancySort(list);
    return list;
}

void QueryProcessor::relevancySort(vector<Document> & list) {
    // used insertion sort
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
    reverse(list.begin(),list.end());
}
