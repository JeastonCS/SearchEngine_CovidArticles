#include "Document.h"

Document::Document(const string &id, int count, const string &pub, const string &u, const string &tit, const vector<string> &auths, const string &text) {
    docID = id;
    wordCount = count;
    publication = pub;
    url = u;
    title = tit;
    authors = auths;
    rawText = text;
}

Document::Document(const Document& doc){
    docID = doc.docID;
    title = doc.title;
    authors = doc.authors;
    publication = doc.publication;
    url = doc.url;
    rawText = doc.rawText;
    wordCount = doc.wordCount;
    tfStat = doc.tfStat;
}
void Document::print() {
    //print title
    if (title.size() == 0) {
        cout << "*no title*" << endl;
        cout << "\t" << docID << endl;
    }
    else {
        stringstream ss(title);
        int size = 0;
        string word;
        while (ss >> word) {
            size += word.size() + 1;
            if (size > 80) {
                cout << "\n\t" << flush;
                size = word.size() + 1;
            }

            cout << word << " ";
        }
        cout << endl;
    }

    //print authors
    for (int i = 0; i < authors.size(); i++) {
        cout << "\t- " << authors[i] << endl;
    }

    cout << "\t" << url << endl;

    cout << "\t" << publication << endl;
}
Document& Document::operator=(const Document& rhs) {

    docID = rhs.docID;
    title = rhs.title;
    authors = rhs.authors;
    publication = rhs.publication;
    url = rhs.url;
    rawText = rhs.rawText;
    wordCount = rhs.wordCount;
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

void Document::populateProcessedWords(const vector<string> &stopWords, vector<DocumentWord> &processedWords) {
    stringstream sstream(rawText);

    wordCount = 0;
    string word;
    while(sstream >> word) {
        stem(word);

        // if not a stop word AND more than 1 letter
        if (word.size() > 1 && (count(stopWords.begin(), stopWords.end(), word) == 0)) {
            DocumentWord docWord(word);

            auto it = find(processedWords.begin(), processedWords.end(), docWord);
            if (it != processedWords.end()) { //already in processedWords
                //increment that word's count
                it->incrementTimesInDoc();
            }
            else {
                //add to processedWords
                processedWords.push_back(docWord);
            }
            //increment total word count in document
            wordCount++;
        }
    }
}

void Document::initializeDocWordsTermFrequency(vector<DocumentWord> &processedWords) {
    for (int i = 0; i < processedWords.size(); i++) {
        processedWords[i].initTermFreq(wordCount);
    }
}

void Document::stem(string &toStem) {
    Porter2Stemmer::trim(toStem);
    Porter2Stemmer::stem(toStem);
}
