#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>

#include <iostream>
using namespace std;

class Document{
public:
    string docID;
    string title;
    vector<string> authors;
    string publication;
    double tfStat;

    Document(string t = "empty") {docID = t;}
    Document(const Document& doc);
    void print();
    Document& operator=(const Document& rhs) ;
    bool operator>(const Document& rhs) ;
    bool operator<(const Document& rhs) ;
    bool operator==(const Document& rhs) ;
    friend ostream& operator<<(ostream &, const Document &);

    void setDocID(string docID) { this->docID = docID; }
    void setPublication(string publication) { this->publication = publication; }
    void setTitle(string title) { this->title = title; }
    void addAuthor(string author) { authors.push_back(author); }
    void setTFStat(double tfStat) { this->tfStat = tfStat; }

    vector<string> & getAuthors() { return authors; }
    string getDocID() { return docID; }
    string getTitle() { return title; }
    string getPublication() { return publication; }
};

#endif //DOCUMENT_H