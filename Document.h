#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "DocumentWord.h"

#include "stemmer/porter2_stemmer.h"

#include <vector>
#include <sstream>

#include <iostream>
using namespace std;

class Document{
private:
    string docID;
    string title;
    string publication;
    string url;

    vector<string> authors;

    string rawText;
//    vector<DocumentWord> docWords;
    int wordCount;

    double tfStat;

public:
    Document(string t = "empty") {docID = t;}
    Document(const string &, int, const string &, const string &, const string &, const vector<string> &, const string &);
    Document(const Document& doc);
    Document& operator=(const Document& rhs);

    //overloaded operators
    bool operator>(const Document& rhs) ;
    bool operator<(const Document& rhs) ;
    bool operator==(const Document& rhs) ;
    friend ostream& operator<<(ostream &, const Document &);

    //misc methods
    void populateProcessedWords(const vector<string> &, vector<DocumentWord> &);
    void initializeDocWordsTermFrequency(vector<DocumentWord> &);
    void print();

    //setters
    void setDocID(const string &docID) { this->docID = docID; }
    void setPublication(const string &publication) { this->publication = publication; }
    void setURL(const string &u) { url = u; }
    void setTitle(const string &title) { this->title = title; }
    void addAuthor(const string &author) { authors.push_back(author); }
    void setRawText(const string &text) { this->rawText = text; }
    void setTFStat(double tfStat) { this->tfStat = tfStat; }

    //getters
    vector<string> & getAuthors() { return authors; }
    string getDocID() { return docID; }
    string getTitle() { return title; }
    string getPublication() { return publication; }
    string getURL() { return url; }
    string getText() { return rawText; }
    int getWordCount() { return wordCount; }
private:
    void stem(string &);
};

#endif //DOCUMENT_H