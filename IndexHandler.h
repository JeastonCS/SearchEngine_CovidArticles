/* Created By: Jack Easton
 * 4/13/2020
 */

#ifndef JSONTEST_INDEXHANDLER_H
#define JSONTEST_INDEXHANDLER_H

#include "WordIndex.h"
#include "AuthorIndex.h"
#include "DocumentProcessor.h"
#include "DocumentWord.h"

#include <sstream>
#include <fstream>
using namespace std;


class IndexHandler {
private:
    WordIndex textIndex;
    AuthorIndex nameIndex;
    DocumentProcessor dProcessor;
public:
    IndexHandler();

    void addProcessorWords();
    void addProcessorAuthors();
    void populateMainWithFile(const char *);
    void populateAuthorsWithFile(const char *);
    void writeMainToFile(const char *);
    void writeAuthorsToFile(const char *);

    vector<string> getWordDocs(string);
    vector<double> getWordFreq(string);
    vector<string> getAuthorDocs(string);

    int getNumUniqueWords() {return textIndex.getNumUniqueWords(); }

    void setProcessor(const DocumentProcessor &);

    void addToWordIndex(string, string, double);
    void addToAuthorIndex(string, string);

    void print();
};


#endif //JSONTEST_INDEXHANDLER_H
