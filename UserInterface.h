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
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class UserInterface {
private:
    IndexHandler handler;
    vector<Document> documents;
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
    bool paginateResultingDocuments(vector<string> &, int);
    void getResultText(vector<string> &, int);
    void getStatistics();
    void getTopFifty();
    void clear();

    string lowercase(string);
};


#endif //JSONTEST_USERINTERFACE_H
