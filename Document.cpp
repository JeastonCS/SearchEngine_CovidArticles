#include "Document.h"

Document::Document(const Document& doc){
    title = doc.title;
    author = doc.author;
    publication = doc.publication;
    tfStat = doc.tfStat;
}
void Document::print() {
    cout << title;
}
Document& Document::operator=(const Document& rhs) {
    if(&rhs == this)
        return *this;

    title = rhs.title;
    author = rhs.author;
    publication = rhs.publication;
    tfStat = rhs.tfStat;
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