//
// Created by Jack Easton on 5/1/2020.
//

#ifndef SEARCHENGINE_METADATAENTRY_H
#define SEARCHENGINE_METADATAENTRY_H

#include <string>
using namespace std;

class MetadataEntry {
private:
    string publicationDate;
    string url;
public:
    MetadataEntry() = default;
    MetadataEntry(string pDate, string u) : publicationDate(pDate), url(u) {}
    MetadataEntry(const MetadataEntry &toCopy) {
        publicationDate = toCopy.publicationDate;
        url = toCopy.url;
    }
    MetadataEntry operator=(const MetadataEntry &rhs) {
        publicationDate = rhs.publicationDate;
        url = rhs.url;
        return *this;
    }

    void setPublicationDate(const string &pubDate) { publicationDate = pubDate; }
    void setURL(const string &url) { this->url = url; }

    string getPublicationDate() { return publicationDate; }
    string getURL() { return url; }
};


#endif //SEARCHENGINE_METADATAENTRY_H
