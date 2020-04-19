#include <iostream>
using namespace std;

class Document{
public:
    string title;
    string author;
    string publication;
    double tfStat;

    Document(string t = "empty") {title = t;}
    Document(const Document& doc);
    void print();
    Document& operator=(const Document& rhs) ;
    bool operator>(const Document& rhs) ;
    bool operator<(const Document& rhs) ;
    bool operator==(const Document& rhs) ;
};