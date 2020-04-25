#include <iostream>
using namespace std;

class Document{
public:
    string docID;
    string title;
    string author;
    string publication;
    double tfStat;

    Document(string t = "empty") {docID = t;}
    Document(const Document& doc);
    void print();
    Document& operator=(const Document& rhs) ;
    bool operator>(const Document& rhs) ;
    bool operator<(const Document& rhs) ;
    bool operator==(const Document& rhs) ;
};