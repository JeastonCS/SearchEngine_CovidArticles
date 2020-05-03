/* Created By: Jack Easton
 * 4/9/2020
 */

#ifndef JSONTEST_DOCUMENTPROCESSOR_H
#define JSONTEST_DOCUMENTPROCESSOR_H

#include "DocumentWord.h"
#include "Word.h"
#include "Document.h"
#include "MetadataEntry.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
using namespace std;


class DocumentProcessor {
private:
    vector<string> stopWords;
    map<string,MetadataEntry> metadata;

    vector<Document> documents;

    //to submit to index handler (put into indexes)
//    string currDocID;
    vector<DocumentWord> processedWords;
public:
    DocumentProcessor() = default;
    DocumentProcessor(const DocumentProcessor &);
    DocumentProcessor & operator=(const DocumentProcessor &);

    //user interface methods
    void populateStopWords(const char *);
    void populateMetadata(const char *);

    bool parseJson(const char *);
    void populateProcessedWords();
    void cleanRawText();
    void clearPWords();

    void addMetadataData();


    vector<DocumentWord> & getCurrProcessedWords() { return processedWords; }
    vector<string> & getCurrAuthors() { return documents[documents.size() - 1].getAuthors(); }
    string getCurrDocID() { return documents[documents.size() - 1].getDocID(); }
    vector<Document> & getDocuments() { return documents; }
private:
    void addMetadataTo(Document &);
    void getSpecificInMetadata(const string &, string &, string &, string &);
};


#endif //JSONTEST_DOCUMENTPROCESSOR_H
