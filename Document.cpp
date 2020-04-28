#include "Document.h"

Document::Document(const Document& doc){
    docID = doc.docID;
    title = doc.title;
    authors = doc.authors;
    publication = doc.publication;
    tfStat = doc.tfStat;
}
void Document::print() {
    cout << title << "\n\t(" << docID << ")" << endl;
}
Document& Document::operator=(const Document& rhs) {
    if(&rhs == this)
        return *this;
    docID = rhs.docID;
    title = rhs.title;
    authors = rhs.authors;
    publication = rhs.publication;
    tfStat = rhs.tfStat;

    return *this;
}
bool Document::operator>(const Document& rhs) {
    return tfStat>rhs.tfStat;
}
bool Document::operator<(const Document& rhs) {
    return tfStat<rhs.tfStat;
}
bool Document::operator==(const Document& rhs) {
    return tfStat==rhs.tfStat;
}

ostream &operator<<(ostream &os, const Document &doc) {
    os << doc.docID << flush;

    return os;
}
