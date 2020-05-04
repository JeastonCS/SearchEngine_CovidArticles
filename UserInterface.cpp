/* Created By: Jack Easton
 * 4/13/2020
 *
 * Citations:
 */

#include "UserInterface.h"

UserInterface::UserInterface()
{
    handler = * new IndexHandler;
    populateStopWords();
    dProcessor = new DocumentProcessor(stopWords);
    qProcessor = new QueryProcessor(stopWords);
}

void UserInterface::interfaceLoop()
{
    const char *wordFile = "word_index.txt";
    const char *authorFile = "author_index.txt";
    const char *documentFile = "documents.txt";

    introduction();

    string command = "help";
    while (command != "exit") {
        //chose path based on user command
        if (command == "help") {
            displayOptions();
        }
        else if (command == "json") {
            populateIndexWithCorpus();
            getStatistics();
        }
        else if (command == "query") {
            submitQuery();
        }
        else if (command == "to file") {
            writeIndexToFile(wordFile, authorFile, documentFile);
        }
        else if (command == "file") {
            populateIndexWithFile(wordFile, authorFile, documentFile);
            getStatistics();
        }
        else if (command == "clear") {
            clear();
        }
        else if (command == "50") {
            getTopFifty();
        }
        else {
            cout << "Invalid command. Try again." << endl;
        }

        //update user command
        cout    << "<<<----------------------------------------------------------------------------->>>" << endl;
        cout    << "Type your command below (or type \"help\" for a list of commands):\n"
                << ">>>" << flush;

        getline(cin, command);
        command = lowercase(command);
        cout << endl;
    }
}

void UserInterface::introduction()
{
    cout    << "                   -====-__-======-__-========-_____-==========-___-===-_\n"
            << "                 _( ___                  _      ___           _          )_\n"
            << "               _(  / __| ___ __ _ _ _ __| |_   | __|_ _  __ _(_)_ _  ___   )\n"
            << "            OO(    \\__ \\/ -_) _` | '_/ _| ' \\  | _|| ' \\/ _` | | ' \\/ -_)   )\n"
            << "           0  (_   |___/\\___\\__,_|_| \\__|_||_| |___|_||_\\__, |_|_||_\\___| _)\n"
            << "         o0     (_                                      |___/           _)\n"
            << "        o         '=-___-===-_____-========-_________-===-_______-====-'\n"
            << "      .o                                _________\n"
            << "     . ______          ______________  |         |      _____\n"
            << "   _()_||__|| ________ |            |  |_________|   __||___||__\n"
            << "  (         | |      | |            | __Y______00_| |_         _|\n"
            << " /-OO----OO\"\"=\"OO--OO\"=\"OO--------OO\"=\"OO-------OO\"=\"OO-------OO\"=P\n"
            << "#####################################################################\n" << endl;

    cout    << "Welcome to the FonteKNOW search engine where we can get you all of the Coronavirus\n"
            << "information you need! Pick from the following list of options to get started.\n"
            << "Each option follows this format: command -> short description of command\n" << endl; ;
}

void UserInterface::displayOptions()
{
    cout    << "Options: \n"
            << " - json    -> populate indexes using directory with json documents\n"
            << " - file    -> populate indexes using files\n"
            << " - query   -> enter a query\n"
            << " - 50      -> get the top 50 most frequent words\n"
            << " - to file -> write current indexes to files\n"
            << " - clear   -> clear current indexes\n"
            << " - exit    -> end session\n"
            << flush;
}

void UserInterface::populateIndexWithCorpus()
{
    //get name of the folder of json documents
    string dir, filepath;
    DIR *dp = NULL;
    struct dirent *dirp;
    struct stat filestat;

    while(dp == NULL) {
        cout    << "Enter the name of your directory (or type \"menu\" to return to the menu):\n"
                << ">>>" << flush;
        getline(cin, dir);
        cout << endl;

        //user wants to ask a new command
        if (dir == "menu") {
            cout << "REDIRECTING...\n" << endl;
            return;
        }

        dp = opendir( dir.c_str() );
        if (dp == NULL) {
            cout << "INVALID directory. Try again." << endl;
        }
    }

    auto start = high_resolution_clock::now();

    while ( (dirp = readdir(dp)) ) {
        filepath = dir + "/" + dirp->d_name;

        //check to make sure filepath is valid
        if (stat( filepath.c_str(), &filestat ))    continue;
        if (S_ISDIR( filestat.st_mode ))        continue;


        //parse json document unless filepath is metadata.csv, then extract metadata's data
        if (!dProcessor->parseJson(filepath.c_str())) {
            dProcessor->populateMetadata(filepath.c_str());
        }
        else {
            dProcessor->populateProcessedWords();
            dProcessor->cleanRawText();

            //add document processor's words to indexes through index handler
            handler.addProcessorWords(dProcessor->getCurrProcessedWords(), dProcessor->getCurrDocID());
            handler.addProcessorAuthors(dProcessor->getCurrAuthors(), dProcessor->getCurrDocID());

            dProcessor->clearPWords();
        }
    }
    // get time
    auto stop = high_resolution_clock::now();
    time1 = time2 = duration_cast<milliseconds>(stop - start).count();

    dProcessor->addMetadataData();

    closedir(dp);

    //initialize instance variable "documents"
    documents = dProcessor->getDocuments();

    //initialize query processor and vector of top 50 most frequent words
    //not done in submit query method so running a query can be faster
    qProcessor->setHandler(handler);
    topFifty = handler.getTopFifty();
}

void UserInterface::populateIndexWithFile(const char *wordIndex, const char *authorIndex, const char *docs) {
    // time and run wordIndex
    auto start = high_resolution_clock::now();
    handler.populateMainWithFile(wordIndex);
    auto stop = high_resolution_clock::now();
    time1 = duration_cast<milliseconds>(stop - start).count();

    // time and run AuthorIndex
    start = high_resolution_clock::now();
    handler.populateAuthorsWithFile(authorIndex);
    stop = high_resolution_clock::now();
    time2 = duration_cast<milliseconds>(stop - start).count();


    handler.getDocumentsWithFile(docs, documents);

    //initialize query processor and vector of top 50 most frequent words
    //not done in submit query method so running a query can be faster
    qProcessor->setHandler(handler);
    topFifty = handler.getTopFifty();
}

void UserInterface::writeIndexToFile(const char *wordOutput, const char *authorOutput, const char *documentOutput)
{
    handler.writeMainToFile(wordOutput);
    handler.writeAuthorsToFile(authorOutput);
    handler.writeDocumentsToFile(documents, documentOutput);
}

void UserInterface::submitQuery()
{
    //get query
    string query;
    cout    << "Enter your query:\n"
            << ">>>" << flush;
    getline(cin, query);

    while (query != "menu") {
        //get query results
        vector<string> qResults;
        // time response
        auto start = high_resolution_clock::now();
        qResults = qProcessor->runQuery(query, documents.size());
        auto stop = high_resolution_clock::now();

        int duration = duration_cast<microseconds>(stop - start).count();
        double micro = duration / 1000000.0;

        //output query results to user
        cout << "Time of query: "<< setprecision(10) << micro << " seconds " << '\n' << endl;
        cout << "Number of documents: " << qResults.size() << "\n" << endl;

        int pagesPerPagination = 5;
        int pageNum = 1;
        string choice = "next";
        bool morePages = paginateResultingDocuments(qResults, pageNum, pagesPerPagination);
        while(morePages && choice == "next") {
            //check to see if user wants to see more documents
            cout    << "Pick a source to display or type \"next\" to see more results (or \"new\" to submit a new query)\n"
                    << ">>>" << flush;
            getline(cin, choice);
            cout << endl;

            //user wants to ask a new command
            if ( isdigit(choice.at(0)) ) {
                getResultText(qResults, stoi(choice));
                choice = "next";
                continue;
            }
            else if (choice == "new"){
                break;
            }
            else if (choice != "next") {
                cout << "invalid choice" << endl;
                choice = "next";
                continue;
            }

            pageNum++;
            paginateResultingDocuments(qResults, pageNum, pagesPerPagination);
        }

        if (choice != "new") {
            do {
                cout << "Pick a source to display (or \"new\" to submit a new query)\n"
                     << ">>>" << flush;
                getline(cin, choice);
                cout << endl;
                if ( isdigit(choice.at(0)) ) {
                    getResultText(qResults, stoi(choice));
                }
                else if (choice != "new") {
                    cout << "invalid" << endl;
                }
            } while (choice != "new");
        }


        cout    << "Enter a new query (or type \"menu\" to return to the menu):\n"
                << ">>>";
        getline(cin, query);
        cout << endl;
    }

    cout << "REDIRECTING...\n" << endl;
}

void UserInterface::getResultText(vector<string> &qResults, int sourceNum) {
    if (sourceNum > 0 && sourceNum <= qResults.size()) {
        auto it = find(documents.begin(), documents.end(), qResults[sourceNum - 1]);
        cout << "\n\"" << it->getTitle() << "\"" << endl;
        cout << "\t" << it->getText() << "\n" << endl;
    }
    else {
        cout << "Invalid source number. Choose another source." << endl;
    }
}

bool UserInterface::paginateResultingDocuments(vector<string> &qResults, int pageNum, int pagesPerPag) {
    cout << "Page " << pageNum << endl;
    cout << "|-------------------------------------------------------------------------------------------|" << endl;

    for (int i = (pageNum - 1) * pagesPerPag; i < qResults.size() && i < pageNum * pagesPerPag; i++) {
        cout << i+1 << ". ";
        auto it = find( documents.begin(), documents.end(), qResults[i] );

        //formatting
        int digits = 0;
        int temp = pageNum;
        while (temp != 0) {
            temp /= 10;
            digits++;
        }

        //print document
        it->print(digits + 2); //1 for period, 1 for space

        if (i+1 < qResults.size() && i+1 < pageNum * pagesPerPag)
            cout << endl;
    }

    cout << "|-------------------------------------------------------------------------------------------|" << endl;
    cout << endl;

    return (pageNum * pagesPerPag) < qResults.size();
}

void UserInterface::getStatistics()
{
    // time of author Index and Word Index
    cout  << "Time to populate Indexes: "<< time1/60000 << " min "<< (time1%60000)*1.0/ 1000 << " secs" << endl;
//    if (time1 == time2)
//        cout  << "Time to populate Indexes: "<< time1/60000 << " min "<< (time1%60000)*1.0/ 1000 << " secs" << endl;
//    else {
//        cout << "Time to populate WordIndex: " << time1/60000 << " min "<< (time1%60000)*1.0/ 1000 << " secs" << endl;
//        cout << "Time to populate AuthorIndex: " << time2/60000 << " min "<< (time2%60000)*1.0/ 1000 << " secs" << endl;
//    }
    cout << endl;

    //document info
    cout << "Number of documents parsed: " << documents.size() << endl;
    int sum = 0;
    for (auto it = documents.begin(); it != documents.end(); it++)
        sum += it->getWordCount();
    int average = sum / documents.size();
    cout << "Average word count per document: " << average << endl;

    //unique element count for each index
    cout << "Number of unique words: " << handler.getNumUniqueWords() << endl;
    cout << "Number of unique authors: " << handler.getNumUniqueAuthors() << endl;

    cout << endl;
}

void UserInterface::getTopFifty()
{
    cout << "Top 50 most frequent words" << endl;
    int i = 1;
    for (int i = 1; i <= topFifty.size(); i++)
        cout << i << ". " << topFifty.at(topFifty.size() - i).getWord() << endl;

    cout << endl;
}

void UserInterface::clear()
{
    handler.clearIndexes();
    documents.clear();
    topFifty.clear();

    dProcessor = new DocumentProcessor (stopWords);
    qProcessor = new QueryProcessor (stopWords);
}

void UserInterface::populateStopWords() {
    ifstream stopWordsFile("StopWords.txt");
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

string UserInterface::lowercase(string command)
{
    string result;
    for (auto iter = command.begin(); iter != command.end(); iter++)
        result += tolower(*iter);

    return result;
}
