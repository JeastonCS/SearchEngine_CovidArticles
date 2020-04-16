/* Created By: Jack Easton
 * 4/13/2020
 */

#include "IndexHandler.h"

IndexHandler::IndexHandler() {
//    textIndex = * new MainIndex;
//    nameIndex = * new AuthorIndex;
    dProcessor = * new DocumentProcessor;
}

void IndexHandler::addProcessorWords() {
    string docID = dProcessor.getDocID();
    vector<string> words = dProcessor.getProcessedWords();

    for (int i = 0; i < words.size(); i++)
        addToMainIndex(docID, words[i]);
}

void IndexHandler::addProcessorAuthors() {
    string docID = dProcessor.getDocID();
    vector<string> authors = dProcessor.getAuthors();

    for (int i = 0; i < authors.size(); i++)
        addToAuthorIndex(docID, authors[i]);
}

//vector<string> IndexHandler::getWordDocs(string word) {
//    return textIndex.getWord(word).getDocIDs();
//}

//vector<string> IndexHandler::getAuthorDocs(string name) {
//    return nameIndex.getAuthor(name).get
//}

void IndexHandler::setProcessor(const DocumentProcessor &other) {
    dProcessor = other;
}

void IndexHandler::addToMainIndex(string docID, string word) {
//    Word indexWord = * new Word(word, docID);
//    textIndex.addWord(indexWord);
}

void IndexHandler::addToAuthorIndex(string docID, string name) {
//    Author author = * new Author(name, docID);
//    nameIndex.addAuthor(author);
}

