/* Created By: Jack Easton
 * 4/9/2020
 *
 * Citations:
 */

#include "DocumentProcessor.h"

DocumentProcessor::DocumentProcessor(const DocumentProcessor &toCopy)
{
    stopWords = toCopy.stopWords;
    metadata = toCopy.metadata;
    documents = toCopy.documents;
    processedWords = toCopy.processedWords;
}

DocumentProcessor &DocumentProcessor::operator=(const DocumentProcessor &rhs) {
    stopWords = rhs.stopWords;
    metadata = rhs.metadata;
    documents = rhs.documents;
    processedWords = rhs.processedWords;

    return *this;
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

    stopWordsFile.close();
}

bool DocumentProcessor::parseJson(const char *fileName) {
    Document doc = *new Document;

    using namespace rapidjson;

    //create ifstream wrapper
    ifstream jsonFile(fileName);
    if (!jsonFile) {
        cout << "could not open json file" << endl;
        exit(1);
    }
    IStreamWrapper isw(jsonFile);

    //create and parse current json document
    rapidjson::Document document;
    document.ParseStream( isw );
    jsonFile.close();

    if (!document.IsObject())
        return false;

    //get and store ID
    doc.setDocID(document["paper_id"].GetString());

    //get and store title
    doc.setTitle(document["metadata"]["title"].GetString());

    //get and store author names
    for (Value &val : document["metadata"]["authors"].GetArray()) {
        string authorName = val["last"].GetString();
        doc.addAuthor(authorName);
    }

    //get and store raw text
    string docText;
    for(Value &val : document["body_text"].GetArray()) {
        docText.append(val["text"].GetString());
    }

    doc.setRawText(docText);

    documents.push_back(doc);

    return true;
}

void DocumentProcessor::clearPWords() {
    processedWords.clear();
}

void DocumentProcessor::populateMetadata(const char *fileName)
{
    ifstream metadataFile(fileName);
    if (!metadataFile) {
        cout << "could not open metadata file" << endl;
        exit(1);
    }

    //add metadata information to documents vector
    string line;
    string docID, publicationDate, url;
    getline(metadataFile, line); //get rid of category line

    getline(metadataFile, line);
    while (!metadataFile.eof()) {
        getSpecificInMetadata(line, docID, publicationDate, url);

        MetadataEntry *value = new MetadataEntry(publicationDate, url);
        metadata.emplace( docID, *value );

        //get a new line
        getline(metadataFile, line);
    }

    metadataFile.close();
}

void DocumentProcessor::getSpecificInMetadata(const string &line, string &docID, string &publicationDate, string &url)
{
    //get position of fist comma of publish date
    auto pos = 0;
    auto next = line.find(',');
    auto length = 0;

    for (int i = 0; i < 17; i++) {
        if (i == 1) {
            length = next - pos - 1;
            docID = line.substr(pos + 1, length);
        }
        else if (i == 8) {
            next = line.find('\"', pos + 2) + 1;
        }
        else if (i == 9) {
            length = next - pos - 1;
            publicationDate = line.substr(pos + 1, length);
        }
        else if (i == 10)
            next = line.find('\"', pos + 2) + 1;

        pos = next;
        next = line.find(',', pos + 1);
    }
    url = line.substr(pos + 1);
}

void DocumentProcessor::addMetadataData()
{
    for (int i = 0; i < documents.size(); i++) {
        addMetadataTo(documents[i]);
    }
}

void DocumentProcessor::addMetadataTo(Document &doc)
{
    MetadataEntry value = metadata.at(doc.getDocID());
    doc.setPublication(value.getPublicationDate());
    doc.setURL(value.getURL());
}

void DocumentProcessor::populateProcessedWords()
{
    documents[documents.size() - 1].populateProcessedWords(stopWords, processedWords);
    documents[documents.size() - 1].initializeDocWordsTermFrequency(processedWords);
}

void DocumentProcessor::cleanRawText()
{
    stringstream ss(documents.at(documents.size() - 1).getText());
    string result;

    string word;
    int i = 0;
    while (ss >> word && i < 300) {
        result += word;
        result += " ";
        i++;
    }
    result += "...";
    documents.at(documents.size() - 1).setRawText(result);
}
