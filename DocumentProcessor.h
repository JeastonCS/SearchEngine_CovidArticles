/* Created By: Jack Easton
 * 4/9/2020
 */

#ifndef JSONTEST_DOCUMENTPROCESSOR_H
#define JSONTEST_DOCUMENTPROCESSOR_H

#include "DocumentWord.h"
#include "Document.h"

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
    int docWordCount;
    vector<DocumentWord> processedWords;
    Document doc;

    vector<string> stopWords;
public:
    DocumentProcessor() = default;
    DocumentProcessor(const char *);
    DocumentProcessor & operator=(const DocumentProcessor &);

    Document & getDocument() { return doc; }
    vector<DocumentWord> & getProcessedWords() { return processedWords; }
    vector<string> & getAuthors() { return doc.getAuthors(); }

    void initializeDocWordsTermFrequency();

    void print();

private:
    void parseJson(const char *);
    void populateStopWords(const char *);
    void populateProcessedWords();

    void stem(string &);
};


#endif //JSONTEST_DOCUMENTPROCESSOR_H
