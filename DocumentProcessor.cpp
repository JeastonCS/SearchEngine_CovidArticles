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
    if (!doc.IsObject())
        return;

    docID = doc["paper_id"].GetString();

    //get and store author names
    for (Value &val : doc["metadata"]["authors"].GetArray()) {
        string authorName = val["last"].GetString();
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

        // if not a stop word AND more than 1 letter
        if (word.size() > 1 && (count(stopWords.begin(), stopWords.end(), word) == 0)) {
            DocumentWord *docWord = new DocumentWord(word);

            if (count(processedWords.begin(), processedWords.end(), word) != 0) { //already in processedWords
                //increment that word's count
                find(processedWords.begin(), processedWords.end(), *docWord)->incrementTimesInDoc();
            }
            else {
                //add to processedWords
                processedWords.push_back(*docWord);
            }
            //increment total word count in document
            docWordCount++;
        }
    }
}

void DocumentProcessor::initializeDocWordsTermFrequency() {
    for (int i = 0; i < processedWords.size(); i++) {
        processedWords[i].initTermFreq(docWordCount);
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

    cout << "Total words: " << docWordCount << endl;
    for (int i = 0; i < processedWords.size(); i++) {
        cout << i + 1 << ": " << processedWords.at(i) << endl;
    }
}
