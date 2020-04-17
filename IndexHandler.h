/* Created By: Jack Easton
 * 4/13/2020
 */

#ifndef JSONTEST_INDEXHANDLER_H
#define JSONTEST_INDEXHANDLER_H

#include "WordIndex.h"
#include "AuthorIndex.h"
#include "DocumentProcessor.h"


class IndexHandler {
private:
    WordIndex textIndex;
    AuthorIndex nameIndex;
    DocumentProcessor dProcessor;
public:
    IndexHandler();

    void addProcessorWords();
//    void addProcessorAuthors();

    vector<string> getWordDocs(string);
//    vector<string> getAuthorDocs(string);

    int getNumUniqueWords() {return textIndex.getNumUniqueWords(); }
    //TODO
    WordIndex & getWordIndex() { return textIndex; }

    void setProcessor(const DocumentProcessor &);

private:
    void addToWordIndex(string, string);
//    void addToAuthorIndex(string, string);
};


#endif //JSONTEST_INDEXHANDLER_H
