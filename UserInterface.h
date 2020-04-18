/* Created By: Jack Easton
 * 4/13/2020
 */

#ifndef JSONTEST_USERINTERFACE_H
#define JSONTEST_USERINTERFACE_H

#include "IndexHandler.h"
#include "QueryProcessor.h"
#include "DocumentProcessor.h"

#include <dirent.h>
#include <sys/stat.h>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class UserInterface {
private:
    IndexHandler handler;
    int numDocsParsed;
    chrono::microseconds parseTime;
public:
    UserInterface();

    void interfaceLoop();

private:
    void introduction();
    void displayOptions();
    void populateIndexWithCorpus();
    void populateIndexWithFile(const char *);
//    void writeIndexToFile(const char *);
    void submitQuery();
    void paginateResultingDocuments(vector<string> &, int);
    void getStatistics();

    string lowercase(string);
};


#endif //JSONTEST_USERINTERFACE_H
