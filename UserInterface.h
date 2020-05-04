/* Created By: Jack Easton
 * 4/13/2020
 */

#ifndef JSONTEST_USERINTERFACE_H
#define JSONTEST_USERINTERFACE_H

#include "IndexHandler.h"
#include "QueryProcessor.h"
#include "DocumentProcessor.h"
#include "Document.h"

#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;
using namespace std::chrono;

class UserInterface {
private:
    IndexHandler handler;
    vector<Document> documents;
    vector<Word> topFifty;
    vector<string> stopWords;

    DocumentProcessor *dProcessor;
    QueryProcessor *qProcessor;

    long time1 = 0; // wordIndex in milliseconds
    long time2 = 0; // authorIndex in milliseconds
public:
    UserInterface();

    void interfaceLoop();

private:
    void introduction();
    void displayOptions();
    void populateIndexWithCorpus();
    void populateIndexWithFile(const char *, const char *, const char *);
    void writeIndexToFile(const char *, const char *, const char *);
    void submitQuery();
    bool paginateResultingDocuments(vector<string> &, int, int);
    void getResultText(vector<string> &, int);
    void getStatistics();
    void getTopFifty();
    void clear();

    void populateStopWords();
    string lowercase(string);
};


#endif //JSONTEST_USERINTERFACE_H
