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

vector<Document> QueryProcessor::runQuery(string query, int numDocs) {
    numOfDocs = numDocs;

    // process/split query
    stringstream ss(query);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> queryOrder(begin, end);

    vector<Document> currentList;
    vector<string> params;

    // use a stack functionality to compute queries similar to infix notation
    // pop words into the parameters and pop operator calls set functions
    while (!queryOrder.empty()){
        string curr = queryOrder[queryOrder.size() - 1];

        if(strcasecmp(curr.c_str(),"AUTHOR") == 0){
            // get Authors and convert to Documents
            vector<vector<Document>> docs;
            for (string& s: params) {
                vector<string> temp = ih.getAuthorDocs(s);
                vector<Document> tempDocs;
                for (string str : temp) {
                    Document x(str);
                    x.tfStat = 0;
                    tempDocs.push_back(x);
                }
                docs.push_back(tempDocs);
            }

            // check current to chain queries
            if (!currentList.empty())
                docs.push_back(currentList);

            // get search term preceding the AUTHOR
            if (queryOrder.size() == 1) {
                cout << "not valid AUTHOR Statement --  term AUTHOR author(s)" << endl;
                break;
            }
            string nextParam = queryOrder[queryOrder.size() - 2];
            stem(nextParam);
            vector<Document> d1 = stringToDoc(nextParam);

            // do set difference and clear
            currentList = getAuthor(d1,docs);
            queryOrder.pop_back();
            params.clear();
        }

        else if(strcasecmp(curr.c_str(),"AND") == 0){
            // stem and initialize
            vector<vector<Document>> docs;
            for (string& s: params) {
                stem(s);
                docs.push_back(stringToDoc(s));
            }

            // check current to chain queries
            if (!currentList.empty())
                docs.push_back(currentList);

            // get set intersection and clear
            currentList = getIntersection(docs);
            params.clear();
        }

        else if(strcasecmp(curr.c_str(),"OR") == 0){
            // stem and initialize
            vector<vector<Document>> docs;
            for (string& s: params) {
                stem(s);
                docs.push_back(stringToDoc(s));
            }

            // check current to chain queries
            if (!currentList.empty())
                docs.push_back(currentList);

            // get set Union and clear
            currentList = getUnion(docs);
            params.clear();
        }

        else if(strcasecmp(curr.c_str(),"NOT") == 0){
            // stem and initialize
            vector<vector<Document>> docs;
            for (string& s: params) {
                stem(s);
                docs.push_back(stringToDoc(s));
            }

            // check current to chain queries
            if (!currentList.empty())
                docs.push_back(currentList);

            // get search term preceding the NOT
            if (queryOrder.size() == 1) {
                cout << "Not Valid NOT statement -- term NOT term(s)" << endl;
                break;
            }
            string nextParam = queryOrder[queryOrder.size() - 2];
            stem(nextParam);
            vector<Document> d1 = stringToDoc(nextParam);

            // do set difference and clear
            currentList = getDifference(d1,docs);
            queryOrder.pop_back();
            params.clear();
        }

        else {
            params.push_back(curr);
        }

        // if query is only one word
        if(params.size() == 1 && queryOrder.size() == 1) {
            stem(params[0]);
            currentList = stringToDoc(params[0]);
            relevancySort(currentList);
        }

        queryOrder.pop_back();
    }

    return currentList;
}

double QueryProcessor::findTFIDRStat(int querySize,int numOfDocs, double termFreq) {
    double tf = termFreq; // find term frequency in doc / total words in doc -> done in DocumentWord
    double idf = log((numOfDocs*1.0) / (querySize) + 1); // log_e(total num of documents / number of documents with word + 1)
    return tf * idf;
}

vector<Document> QueryProcessor::getUnion(vector<vector<Document>>& docs){
    vector<Document> resultList = docs[0];

    for (int i = 1; i < docs.size(); i++){
        for (Document &x : docs[i]) {
            bool found = false;
            for (Document &y: resultList) {
                if (x.docID == y.docID) {
                    y.tfStat = y.tfStat + x.tfStat; // change to make more precise
                    found = true;
                    break;
                }
            }
            if (!found)
                resultList.push_back(x);
        }
    }
    relevancySort(resultList);
    return resultList;
}

vector<Document> QueryProcessor::getIntersection(vector<vector<Document>>& docs) {
    vector<Document> currList = docs[0];
    vector<Document> resultList;

    for (int i = 1; i < docs.size(); ++i) {
        for (Document &x : docs[i]) {
            for (Document &y : currList) {
                if (x.docID == y.docID) {
                    x.tfStat = x.tfStat + y.tfStat; // edit to make more precise
                    resultList.push_back(x);
                }
            }
        }

        currList = resultList;
        resultList.clear();
    }

    relevancySort(currList);
    return currList;
}

vector<Document> QueryProcessor::getDifference(vector<Document>& terms, vector<vector<Document>>& docs) {
    vector<Document> currList = getUnion(docs);
    vector<Document> resultList;

    for (Document& x : terms) {
        bool found = false;
        for (Document& y: currList) {
            if(x.docID == y.docID) {
                found = true;
                break;
            }
        }
        if(!found) {
            resultList.push_back(x);
        }
    }

    relevancySort(resultList);
    return resultList;
}


vector<Document> QueryProcessor::getAuthor(vector<Document>& terms, vector<vector<Document>>& docs) {
    vector<Document> currList = getUnion(docs);
    vector<Document> resultList;

    for (Document& x : terms) {
        for (Document& y : currList) {
            if (x.docID == y.docID)
                resultList.push_back(x);
        }
    }

    relevancySort(resultList);
    return resultList;
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
            j--;
        }
        list[j + 1] = key;
    }
    reverse(list.begin(),list.end());
}
