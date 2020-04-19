/* Created By: Jack Easton
 * 4/9/2020
 */

#ifndef JSONTEST_DOCUMENTPROCESSOR_H
#define JSONTEST_DOCUMENTPROCESSOR_H

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "stemmer/porter2_stemmer.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;


class DocumentProcessor {
private:
    string docText;
    vector<string> processedWords;
//    vector<string> authors;
    string docID;

    vector<string> stopWords;
public:
    DocumentProcessor() = default;
    DocumentProcessor(const char *);
    DocumentProcessor & operator=(const DocumentProcessor &);

    string & getDocID() { return docID; }
    vector<string> & getProcessedWords() { return processedWords; }
//    vector<string> & getAuthors() { return authors; }

    void print();

private:
    void parseJson(const char *);
    void populateStopWords(const char *);
    void populateProcessedWords();

    void stem(string &);
};


#endif //JSONTEST_DOCUMENTPROCESSOR_H
