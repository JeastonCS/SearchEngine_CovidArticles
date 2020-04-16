/* Created By: Jack Easton
 * 4/13/2020
 */

#include "IndexHandler.h"

IndexHandler::IndexHandler() {
    textIndex = * new WordIndex;
    nameIndex = * new AuthorIndex;
    dProcessor = * new DocumentProcessor;
}

void IndexHandler::addProcessorWords() {
    string docID = dProcessor.getDocID();
    vector<string> words = dProcessor.getProcessedWords();

    for (int i = 0; i < words.size(); i++)
        addToWordIndex(docID, words[i]);
}

void IndexHandler::addProcessorAuthors() {
    string docID = dProcessor.getDocID();
    vector<string> authors = dProcessor.getAuthors();

    for (int i = 0; i < authors.size(); i++)
        addToAuthorIndex(docID, authors[i]);
}

vector<string> IndexHandler::getWordDocs(string word) {
    return textIndex.getWord(word).getDocIDs();
}

vector<string> IndexHandler::getAuthorDocs(string name) {
    return nameIndex.getAuthor(name);
}

void IndexHandler::setProcessor(const DocumentProcessor &other) {
    dProcessor = other;
}

void IndexHandler::addToWordIndex(string docID, string word) {
    textIndex.addWord(word, docID);
}

void IndexHandler::addToAuthorIndex(string docID, string name) {
    nameIndex.addAuthor(name, docID);
}

