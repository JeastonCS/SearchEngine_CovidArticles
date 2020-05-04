// Yash Sinha
//

#include "QueryProcessor.h"
#include <iterator>
#include <math.h>
#include <algorithm>
#include <queue>
#include "stemmer/porter2_stemmer.h"


//QueryProcessor::QueryProcessor() { }

void QueryProcessor::stem(string & str) {
    Porter2Stemmer::trim(str);
    Porter2Stemmer::stem(str);
}

vector<DocStat> QueryProcessor::stringToDoc(const string& word) {
    vector<string> docs = ih.getWordDocIDs(word);
    vector<double> freq = ih.getWordFreq(word);
    vector<DocStat> final;
    int i = 0;
    // pair strings with tfidr stats
    for (string& s:docs) {
        double stat = findTFIDRStat(docs.size(),numOfDocs,freq[i]);
        DocStat d(s,stat);
        final.push_back(d);
        i++;
    }
    return final;
}

vector<string> QueryProcessor::runQuery(string query, int numDocs) {
    // set numOfDocs
    numOfDocs = numDocs;

    // process/split query
    stringstream ss(query);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> split(begin, end);

    // create queue and list
    queue<string> queryOrder;
    vector<DocStat> currentList;
    for (string& s: split)
        queryOrder.push(s);

    // use a queue functionality to compute queries similar to math prefix/Polish notation
    // pop words as the parameters until reach end of queue or KEYWORD
    while (!queryOrder.empty()){
        string curr = queryOrder.front();
        queryOrder.pop();

        if(curr == "AUTHOR"){
            // stem and initialize
            vector<vector<DocStat>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                   curr != "AND" &&
                   curr != "OR" &&
                   curr != "AUTHOR" &&
                   curr != "NOT")
            {
                vector<string> authors = ih.getAuthorDocIDs(curr);
                vector<DocStat> temp;

                // make DocStats from list of author Docs
                for (string s: authors)
                    temp.emplace_back(s);

                // add to current query logic
                docs.push_back(temp);
                queryOrder.pop();
                if(!queryOrder.empty())
                    curr = queryOrder.front();
            }

            // get search term preceding the AUTHOR
            if (currentList.empty()) {
                cout << "not valid AUTHOR Statement--  [term] AUTHOR [author(s)]" << endl;
                break;
            }

            // do Author Set Intersection
            currentList = getAuthor(currentList,docs);
        }

        else if(curr == "AND"){
            // stem and initialize
            vector<vector<DocStat>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                   curr != "AND" &&
                   curr != "OR" &&
                   curr != "AUTHOR" &&
                   curr != "NOT")
            {
                stem(curr);
                if(find(stopWords.begin(), stopWords.end(), curr) == stopWords.end())
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

        else if(curr == "OR"){
            // stem and initialize
            vector<vector<DocStat>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                   curr != "AND" &&
                   curr != "OR" &&
                   curr != "AUTHOR" &&
                   curr != "NOT")
            {
                stem(curr);
                if(find(stopWords.begin(), stopWords.end(), curr) == stopWords.end())
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

        else if(curr == "NOT"){
            // stem and initialize
            vector<vector<DocStat>> docs;
            curr = queryOrder.front();

            // get terms until keyword
            while (!queryOrder.empty() &&
                   curr != "AND" &&
                   curr != "OR" &&
                   curr != "AUTHOR" &&
                   curr != "NOT")
            {
                stem(curr);
                if(find(stopWords.begin(), stopWords.end(), curr) == stopWords.end())
                    docs.push_back(stringToDoc(curr));
                queryOrder.pop();
                if(!queryOrder.empty())
                    curr = queryOrder.front();
            }

            // get search term preceding the NOT
            if (currentList.empty()) {
                cout << "Not Valid NOT statement--  [term] NOT [term(s)]" << endl;
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
                if(find(stopWords.begin(), stopWords.end(), curr) == stopWords.end())
                    currentList = (stringToDoc(curr));
                relevancySort(currentList);

            } else {
                // unintended list at start with no keyword
                vector<vector<DocStat>> docs;
                stem(curr);
                docs.push_back(currentList);
                if(find(stopWords.begin(), stopWords.end(), curr) == stopWords.end())
                    docs.push_back(stringToDoc(curr));

                // DEFAULT is a union of these words
                getUnion(docs);
            }
        }
    }

    // convert back to string to get Documents in UI
    vector<string> finalList;
    for (DocStat& d : currentList)
        finalList.push_back(d.docID);

    return finalList;
}

double QueryProcessor::findTFIDRStat(int querySize,int numOfDocs, double termFreq) {
    double tf = termFreq; // find term frequency in doc / total words in doc -> done in DocumentWord
    double idf = log((numOfDocs*1.0) / (querySize) + 1); // log_e(total num of documents / number of documents with word + 1)
    return tf * idf;
}

vector<DocStat> QueryProcessor::getUnion(vector<vector<DocStat>>& docs){
    vector<DocStat> resultList = docs[0];

    for (int i = 1; i < docs.size(); i++){
        for (DocStat &x : docs[i]) {
            bool found = false;
            for (DocStat &y: resultList) {
                if (x.docID == y.docID) {
                    y.tfidf += x.tfidf;
                    y.tfidf /= 2;
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

vector<DocStat> QueryProcessor::getIntersection(vector<vector<DocStat>>& docs) {
    vector<DocStat> currList = docs[0];
    vector<DocStat> resultList;

    for (int i = 1; i < docs.size(); ++i) {
        for (DocStat &x : docs[i]) {
            for (DocStat &y : currList) {
                if (x.docID == y.docID) {
                    x.tfidf += y.tfidf;
                    x.tfidf /= 2;
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

vector<DocStat> QueryProcessor::getDifference(vector<DocStat>& terms, vector<vector<DocStat>>& docs) {
    vector<DocStat> currList = getUnion(docs);
    vector<DocStat> resultList;

    for (DocStat& x : terms) {
        bool found = false;
        for (DocStat& y: currList) {
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


vector<DocStat> QueryProcessor::getAuthor(vector<DocStat>& terms, vector<vector<DocStat>>& docs) {
    vector<DocStat> currList = getUnion(docs);
    vector<DocStat> resultList;

    for (DocStat& x : terms) {
        for (DocStat& y : currList) {
            if (x.docID == y.docID)
                resultList.push_back(x);
        }
    }

    relevancySort(resultList);
    return resultList;
}

void QueryProcessor::relevancySort(vector<DocStat> & list) {
    // used insertion sort

    int size = list.size();
    int i, j;
    DocStat key;

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