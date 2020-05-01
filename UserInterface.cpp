/* Created By: Jack Easton
 * 4/13/2020
 *
 * Citations:
 */

#include "UserInterface.h"

UserInterface::UserInterface()
{
    handler = * new IndexHandler;

    numDocsParsed = 0;
}

void UserInterface::interfaceLoop()
{
    introduction();

    const char *wordFile = "word_index.txt";
    const char *authorFile = "author_index.txt";

    string command = "command list";
    while (command != "quit") {
        //chose path based on user command
        if (command == "command list") {
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
            writeIndexToFile(wordFile, authorFile);
        }
        else if (command == "file") {
            populateIndexWithFile(wordFile, authorFile);
        }
        else if (command == "clear") {
            handler.clearIndexes();
            clear(wordFile, authorFile);
        }
        else {
            cout << "Invalid command. Try again." << endl;
        }

        //update user command
        cout    << "<<<----------------------------------------------------------------------------->>>" << endl;
        cout    << "Type your command below (or type \"command list\" for a list of commands):\n"
                << ">>> " << flush;

        getline(cin, command);
        command = lowercase(command);
        cout << endl;
    }
}

void UserInterface::introduction()
{
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
            << " - to file -> write current indexes to files\n"
            << " - clear   -> clear current indexes\n"
            << " - quit    -> quit session\n"
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
        cout    << "Enter the name of your directory (or type \"new command\" to submit a new command):\n"
                << ">>> " << flush;
        getline(cin, dir);
        cout << endl;

        //user wants to ask a new command
        if (dir == "new command") {
            cout << "REDIRECTING...\n" << endl;
            return;
        }

        dp = opendir( dir.c_str() );
        if (dp == NULL) {
            cout << "INVALID directory. Try again." << endl;
        }
    }

    dp = opendir(dir.c_str());
    if (dp == NULL) {
        cout << "invalid directory" << endl;
        exit(1);
    }

    while ( (dirp = readdir(dp)) ) {
        filepath = dir + "/" + dirp->d_name;

        //check to make sure filepath is valid
        if (stat( filepath.c_str(), &filestat ))    continue;
        if (S_ISDIR( filestat.st_mode ))        continue;

        //create DocumentProcessor with current json and add its words/authors
        DocumentProcessor dProcessor(filepath.c_str());
        dProcessor.initializeDocWordsTermFrequency();
        handler.setProcessor(dProcessor);
        handler.addProcessorWords();
        handler.addProcessorAuthors();

        //increment number of parsed documents
        numDocsParsed++;
    }

    closedir(dp);
}

void UserInterface::populateIndexWithFile(const char *wordIndex, const char *authorIndex) {
    handler.populateMainWithFile(wordIndex);
    handler.populateAuthorsWithFile(authorIndex);
}

void UserInterface::writeIndexToFile(const char *wordOutput, const char *authorOutput)
{
    handler.writeMainToFile(wordOutput);
    handler.writeAuthorsToFile(authorOutput);
}

void UserInterface::submitQuery()
{
    QueryProcessor *qProcessor = new QueryProcessor(handler);
    //get query
    string query;
    cout    << "Enter your query:\n"
            << ">>> " << flush;
    getline(cin, query);

    while (query != "new command") {
        //get query results
        vector<Document> documents;
        documents = qProcessor->runQuery(query, numDocsParsed);

        //output query results to user
        cout << "Number of documents: " << documents.size() << '\n' << endl;

        int pageNum = 1;
        while(paginateResultingDocuments(documents, pageNum)) {
            //check to see if user wants to see more documents
            cout    << "View next page? (\"yes\" or \"no\")\n"
                    << ">>> " << flush;
            string choice;
            getline(cin, choice);
            cout << endl;
            lowercase(choice);

            //user wants to ask a new command
            if (choice == "no") {
                return;
            }

            pageNum++;
        }

        cout    << "Enter a new query (or \"new command\" to go back to the main menu):\n"
                << ">>> ";
        getline(cin, query);
        cout << endl;
    }
}

bool UserInterface::paginateResultingDocuments(vector<Document> &documents, int pageNum) {
    cout << "Page " << pageNum << endl;
    for (int i = (pageNum - 1) * 15; i < documents.size() && i < pageNum * 15; i++) {
        cout << i+1 << ". ";
        documents[i].print();
    }
    cout << endl;

    return (pageNum * 15) < documents.size();
}

void UserInterface::getStatistics()
{
    cout << "Number of documents parsed: " << numDocsParsed << endl;
    cout << "Number of unique words: " << handler.getNumUniqueWords() << endl;
    cout << endl;
}

void UserInterface::clear(const char *wordFileName, const char *authorFileName)
{
    ifstream wordFile(wordFileName), authorFile(authorFileName);
    if (!wordFile || !authorFile) {
        cout << "could not open index files to clear" << endl;
        exit(1);
    }

    wordFile.close();
    authorFile.close();
}

string UserInterface::lowercase(string command)
{
    string result;
    for (auto iter = command.begin(); iter != command.end(); iter++)
        result += tolower(*iter);

    return result;
}
