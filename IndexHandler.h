/* Created By: Jack Easton
 * 4/13/2020
 */

#ifndef JSONTEST_INDEXHANDLER_H
#define JSONTEST_INDEXHANDLER_H

//#include "Word.h"
//#include "Author.h"
//#include "MainIndex.h"
//#include "AuthorIndex.h"
#include "DocumentProcessor.h"


class IndexHandler {
private:
//    MainIndex textIndex;
//    AuthorIndex nameIndex;
    DocumentProcessor dProcessor;
public:
    IndexHandler();

    void addProcessorWords();
    void addProcessorAuthors();

//    vector<string> getWordDocs(string);
//    vector<string> getAuthorDocs(string);

    int getNumUniqueWords() {return /*textIndex.getNumUniqueWords()*/ 0; }

    void setProcessor(const DocumentProcessor &);

private:
    void addToMainIndex(string, string);
    void addToAuthorIndex(string, string);
};


#endif //JSONTEST_INDEXHANDLER_H
