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
    Document docID = dProcessor.getDocument();
    vector<DocumentWord> words = dProcessor.getProcessedWords();

    for (int i = 0; i < words.size(); i++)
        addToWordIndex(docID, words[i].getWord(), words[i].getTermFreq());
}

void IndexHandler::addProcessorAuthors() {
    Document docID = dProcessor.getDocument();
    vector<string> authors = dProcessor.getAuthors();

    for (int i = 0; i < authors.size(); i++)
        addToAuthorIndex(docID, authors[i]);
}

void IndexHandler::populateMainWithFile(const char *fileName)
{
    ifstream input(fileName);
    if (!input) {
        cout << "could not open file with index data" << endl;
        exit(1);
    }

    string line;
    while (!input.eof()) {
        getline(input, line);
        stringstream stream(line);

        //extract data and add to index
        string word;
        stream >> word;

        string docID;
        double termFreq;
        while (stream >> docID) {
            stream >> termFreq;
            addToWordIndex(docID, word, termFreq);
        }
    }

    input.close();
}

void IndexHandler::populateAuthorsWithFile(const char *fileName)
{
    ifstream input(fileName);
    if (!input) {
        cout << "could not open file with index data" << endl;
        exit(1);
    }

    string line;
    while (!input.eof()) {
        getline(input, line);
        stringstream stream(line);

        //extract data and add to index
        string author;
        stream >> author;

        string docID;
        while (stream >> docID) {
            addToAuthorIndex(docID, author);
        }
    }

    input.close();
}

void IndexHandler::writeMainToFile(const char *outputName)
{
    textIndex.writeToFile(outputName);
}

void IndexHandler::writeAuthorsToFile(const char *outputName)
{
    nameIndex.writeToFile(outputName);
}

vector<Document> IndexHandler::getWordDocs(string word)
{
    return textIndex.getWordDocs(word);
}

vector<string> IndexHandler::getAuthorDocs(string name) {
    return nameIndex.getAuthor(name);
}

vector<double> IndexHandler::getWordFreq(string word) {
    return textIndex.getTermFreq(word);
}

void IndexHandler::setProcessor(const DocumentProcessor &other) {
    dProcessor = other;
}

void IndexHandler::addToWordIndex(const Document &doc, string word, double termFreq) {
    textIndex.addWord(word, doc, termFreq);
}

void IndexHandler::addToAuthorIndex(const Document &docID, string name) {
    nameIndex.addAuthor(name, docID);
}

void IndexHandler::clearIndexes() {
    textIndex.clear();
    nameIndex.clear();
}

void IndexHandler::print() {
    textIndex.print();
}

