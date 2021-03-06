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
public:
    IndexHandler();
    IndexHandler & operator=(const IndexHandler &);

    void addProcessorWords(vector<DocumentWord> &, const string &);
    void addProcessorAuthors(vector<string> &, const string &);
    void populateMainWithFile(const char *);
    void populateAuthorsWithFile(const char *);
    void getDocumentsWithFile(const char *, vector<Document> &);
    void writeMainToFile(const char *);
    void writeAuthorsToFile(const char *);
    void writeDocumentsToFile(vector<Document> &, const char *);

    vector<string> getWordDocIDs(string);
    vector<double> getWordFreq(string);
    vector<string> getAuthorDocIDs(string);

    int getNumUniqueWords() {return textIndex.getNumUniqueWords(); }
    int getNumUniqueAuthors() {return nameIndex.getNumUniqueAuthors(); }
    vector<Word> getTopFifty() { return textIndex.getTopFifty(); }

    void addToWordIndex(const string &, string, double);
    void addToAuthorIndex(const string &, string);

    void clearIndexes();

    void print();
};


#endif //JSONTEST_INDEXHANDLER_H
