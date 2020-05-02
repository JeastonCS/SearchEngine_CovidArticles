#include "QueryProcessor.h"
#include <iterator>
#include <math.h>
#include <algorithm>
#include <queue>
#include "stemmer/porter2_stemmer.h"
QueryProcessor::QueryProcessor(const IndexHandler & handler) {
    ih = handler;
}

void QueryProcessor::stem(string & str) {
    Porter2Stemmer::trim(str);
    Porter2Stemmer::stem(str);
}

vector<Document> QueryProcessor::stringToDoc(const string word) {
    vector<Document> docs = ih.getWordDocs(word);
    vector<double> freq = ih.getWordFreq(word);

    int i = 0;
    for (Document& s:docs) {
        s.tfStat = findTFIDRStat(docs.size(),numOfDocs,freq[i]);
        i++;
    }
    return docs;
}

vector<Document> QueryProcessor::runQuery(string query, int numDocs) {
    // set numOfDocs
    numOfDocs = numDocs;

    // process/split query
    stringstream ss(query);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> split(begin, end);

    // create queue and list
    queue<string> queryOrder;
    vector<Document> currentList;
    for (string& s: split)
        queryOrder.push(s);

    // use a queue functionality to compute queries similar to math prefix/Polish notation
    // pop words as the parameters until reach end of queue or KEYWORD
    while (!queryOrder.empty()){
        string curr = queryOrder.front();
        queryOrder.pop();

        if(strcasecmp(curr.c_str(),"AUTHOR") == 0){
            // stem and initialize
            vector<vector<Document>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                   strcasecmp(curr.c_str(),"AND") != 0 &&
                   strcasecmp(curr.c_str(),"OR") != 0 &&
                   strcasecmp(curr.c_str(),"AUTHOR")!= 0 &&
                   strcasecmp(curr.c_str(),"NOT") != 0)
            {
                docs.push_back(ih.getAuthorDocs(curr));
                queryOrder.pop();
                if(!queryOrder.empty())
                    curr = queryOrder.front();
            }

            // get search term preceding the AUTHOR
            if (currentList.empty()) {
                cout << "not valid AUTHOR Statement --  term AUTHOR author(s)" << endl;
                break;
            }

            // do Author Set Intersection
            currentList = getAuthor(currentList,docs);
        }

        else if(strcasecmp(curr.c_str(),"AND") == 0){
            // stem and initialize
            vector<vector<Document>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                    strcasecmp(curr.c_str(),"AND") != 0 &&
                    strcasecmp(curr.c_str(),"OR") != 0 &&
                    strcasecmp(curr.c_str(),"AUTHOR")!= 0 &&
                    strcasecmp(curr.c_str(),"NOT") != 0)
            {
                stem(curr);
                docs.push_back(stringToDoc(curr));
                queryOrder.pop();
                if(!queryOrder.empty())
                    curr = queryOrder.front();
            }

            // check current to chain queries
            if (!currentList.empty())
                docs.push_back(currentList);

            // get set intersection
            currentList = getIntersection(docs);
        }

        else if(strcasecmp(curr.c_str(),"OR") == 0){
            // stem and initialize
            vector<vector<Document>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                   strcasecmp(curr.c_str(),"AND") != 0 &&
                   strcasecmp(curr.c_str(),"OR") != 0 &&
                   strcasecmp(curr.c_str(),"AUTHOR") != 0 &&
                   strcasecmp(curr.c_str(),"NOT") != 0)
            {
                stem(curr);
                docs.push_back(stringToDoc(curr));
                queryOrder.pop();
                if(!queryOrder.empty())
                    curr = queryOrder.front();
            }

            // check current to chain queries
            if (!currentList.empty())
                docs.push_back(currentList);

            // get set intersection
            currentList = getUnion(docs);
        }

        else if(strcasecmp(curr.c_str(),"NOT") == 0){
            // stem and initialize
            vector<vector<Document>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                   strcasecmp(curr.c_str(),"AND") != 0 &&
                   strcasecmp(curr.c_str(),"OR") != 0 &&
                   strcasecmp(curr.c_str(),"AUTHOR")!= 0 &&
                   strcasecmp(curr.c_str(),"NOT") != 0)
            {
                stem(curr);
                docs.push_back(stringToDoc(curr));
                queryOrder.pop();
                if(!queryOrder.empty())
                    curr = queryOrder.front();
            }

            // get search term preceding the NOT
            if (currentList.empty()) {
                cout << "Not Valid NOT statement -- term NOT term(s)" << endl;
                break;
            }

            // do set difference
            currentList = getDifference(currentList,docs);
        }
        //start with word and store in currList
        else {
            if (currentList.empty()) {
                // first word in query
                stem(curr);
                currentList = stringToDoc(curr);
                relevancySort(currentList);

            } else {
                // unintended list at start with no keyword
                vector<vector<Document>> docs;
                stem(curr);
                docs.push_back(currentList);
                docs.push_back(stringToDoc(curr));

                // DEFAULT is a union of these words
                getUnion(docs);
            }
        }
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
                    y.tfStat += x.tfStat; // change to make more precise
                    y.tfStat /= 2;
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
                    x.tfStat += y.tfStat; // edit to make more precise
                    x.tfStat /= 2;
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
