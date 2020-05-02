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

    vector<Document> getWordDocs(string);
    vector<double> getWordFreq(string);
    vector<Document> getAuthorDocs(string);

    int getNumUniqueWords() {return textIndex.getNumUniqueWords(); }
    int getNumUniqueAuthors() {return nameIndex.getNumUniqueAuthors(); }

    void setProcessor(const DocumentProcessor &);

    void addToWordIndex(const Document &, string, double);
    void addToAuthorIndex(const Document &, string);

    void clearIndexes();

    void print();
};


#endif //JSONTEST_INDEXHANDLER_H
