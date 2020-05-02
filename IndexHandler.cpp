/* Created By: Jack Easton
 * 4/13/2020
 */

#include "IndexHandler.h"

IndexHandler::IndexHandler() {
    textIndex = * new WordIndex;
    nameIndex = * new AuthorIndex;
//    dProcessor = * new DocumentProcessor;
}

void IndexHandler::addProcessorWords(vector<DocumentWord> &words, const string &docID) {
    for (int i = 0; i < words.size(); i++)
        textIndex.addWord(words[i].getWord(), docID, words[i].getTermFreq());
}

void IndexHandler::addProcessorAuthors(vector<string> &authors, const string &docID) {
    for (int i = 0; i < authors.size(); i++) {
        addToAuthorIndex(docID, authors[i]);
    }
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

vector<Document> IndexHandler::getDocumentsWithFile(const char *fileName) {
    ifstream docsFile(fileName);
    if (!docsFile) {
        cout << "could not open documents file to read from" << endl;
        exit(1);
    }

    vector<Document> documents;

    string ID, publication, url, title, rawText;
    string line, author;
    vector<string> authors;

    docsFile >> ID;
    while(!docsFile.eof()) {
        docsFile >> publication;
        docsFile >> url;
        getline(docsFile, title);

        getline(docsFile, line);
        stringstream ss(line);
        while (ss >> author)
            authors.push_back(author);

        getline(docsFile, rawText);

        //create Document object with persistence file data
        Document doc(ID, publication, url, title, authors, rawText);

        //add current document to vector of documents
        documents.push_back(doc);
    }

    docsFile.close();

    return documents;
}

void IndexHandler::writeMainToFile(const char *outputName)
{
    textIndex.writeToFile(outputName);
}

void IndexHandler::writeAuthorsToFile(const char *outputName)
{
    nameIndex.writeToFile(outputName);
}

void IndexHandler::writeDocumentsToFile(vector<Document> &documents, const char *outputName)
{
    ofstream documentFile(outputName);
    if (!documentFile) {
        cout << "could not open document file to write to" << endl;
        exit(1);
    }

    for (Document& doc : documents) {
        documentFile << doc.getDocID() << " " << doc.getPublication() << " " << doc.getURL() << " " << doc.getTitle() << endl;
        for (string &author: doc.getAuthors()) {
            documentFile << author << " ";
        }
        documentFile << endl;
        documentFile << doc.getText() << endl;
    }

    documentFile.close();
}

vector<string> IndexHandler::getWordDocIDs(string word)
{
    return textIndex.getWordDocIDs(word);
}
//
//vector<Document> &IndexHandler::getWordDocs(string word)
//{
//
//}

vector<string> IndexHandler::getAuthorDocIDs(string name) {
    return nameIndex.getAuthor(name);
}

vector<double> IndexHandler::getWordFreq(string word) {
    return textIndex.getTermFreq(word);
}

//void IndexHandler::setProcessor(const DocumentProcessor &other) {
//    dProcessor = other;
//}

void IndexHandler::addToWordIndex(const string &docID, string word, double termFreq) {
    textIndex.addWord(word, docID, termFreq);
}

void IndexHandler::addToAuthorIndex(const string &docID, string name) {
    nameIndex.addAuthor(name, docID);
}

void IndexHandler::clearIndexes() {
    textIndex.clear();
    nameIndex.clear();
}

void IndexHandler::print() {
    textIndex.print();
}

