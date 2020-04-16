/* Created By: Jack Easton
 * 4/9/2020
 *
 * Citations:
 */

#include "DocumentProcessor.h"

DocumentProcessor::DocumentProcessor(const char *fileName) {
    parseJson(fileName);
    populateStopWords("StopWords.txt");
    populateProcessedWords();
}

DocumentProcessor &DocumentProcessor::operator=(const DocumentProcessor &rhs) {
    docText = rhs.docText;
    processedWords = rhs.processedWords;
    authors = rhs.authors;
    docID = rhs.docID;

    return *this;
}

void DocumentProcessor::parseJson(const char *fileName) {
    using namespace rapidjson;

    //create ifstream wrapper
    ifstream jsonFile(fileName);
    if (!jsonFile) {
        cout << "could not open json file" << endl;
        exit(1);
    }
    IStreamWrapper isw(jsonFile);

    //create and parse your document
    Document doc;
    doc.ParseStream( isw );
    jsonFile.close();

    //get and store ID
    docID = doc["paper_id"].GetString();

    //get and store author names
    for (Value &val : doc["metadata"]["authors"].GetArray()) {
        // format = "first last"
        string authorName = val["first"].GetString();
        authorName += " ";
        authorName += val["last"].GetString();

        authors.push_back(authorName);
    }

    //get and store raw text
    for(Value &val : doc["body_text"].GetArray()) {
        docText += val["text"].GetString();
    }
}

void DocumentProcessor::populateStopWords(const char *stopFileName) {
    ifstream stopWordsFile(stopFileName);
    if (!stopWordsFile) {
        cout << "could not open stop words file" << endl;
        exit(1);
    }

    while (!stopWordsFile.eof()) {
        string word;
        stopWordsFile >> word;

        stopWords.push_back(word);
    }

    string stopWord;
    while (stopWordsFile >> stopWord) {
        stopWords.push_back(stopWord);
    }
}

void DocumentProcessor::populateProcessedWords() {
    stringstream sstream(docText);

    string word;
    while(sstream >> word) {
        stem(word);
        if ( (word.size() > 1) && (count(processedWords.begin(), processedWords.end(), word) == 0) && (count(stopWords.begin(), stopWords.end(), word) == 0) )
            processedWords.push_back(word);
    }
}

void DocumentProcessor::stem(string &toStem) {
    Porter2Stemmer::trim(toStem);
    Porter2Stemmer::stem(toStem);
}

void DocumentProcessor::print() {
    cout << docID << endl;
    for (int i = 0; i < authors.size(); i++) {
        cout << "\t- " << authors.at(i) << endl;
    }

    for (int i = 0; i < processedWords.size(); i++) {
        cout << i + 1 << ": " << processedWords.at(i) << endl;
    }

    cout << docText << endl;
}
