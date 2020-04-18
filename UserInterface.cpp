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

    string command = "command list";
    while (command != "quit") {
        //chose path based on user command
        if (command == "command list") {
            displayOptions();
        }
        else if (command == "json") {
            time_t start = time(nullptr);
            populateIndexWithCorpus();
            time_t end = time(nullptr);
            parseTime = difftime(end, start);
        }
        else if (command == "query") {
            submitQuery();
        }
        else if (command == "stats") {
            getStatistics();
        }
        else {
            cout << "Invalid command. Try again." << endl;
        }

        //update user command
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
            << " - quit  -> quit session\n"
            << " - json  -> populate indexes using directory with json documents\n"
            << " - query -> enter a query\n"
            << " - stats -> get a list of statistics of the search engine\n"
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
                << ">>>" << flush;
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

    while ( (dirp = readdir(dp)) ) {
        filepath = dir + "/" + dirp->d_name;

        //check to make sure filepath is valid
        if (stat( filepath.c_str(), &filestat )) continue;
        if (S_ISDIR( filestat.st_mode ))         continue;

        //create DocumentProcessor with current json and add its words/authors
        DocumentProcessor dProcessor = * new DocumentProcessor(filepath.c_str());
        handler.setProcessor(dProcessor);
        handler.addProcessorWords();
        handler.addProcessorAuthors();

        //increment number of parsed documents
        numDocsParsed++;
    }

    closedir(dp);
}

void UserInterface::submitQuery()
{
    QueryProcessor *qProcessor = new QueryProcessor(handler);
    //get query
    string query;
    cout    << "Enter your query:\n"
            << ">>> ";
    getline(cin, query);
    cout << endl;

    //get query results
    vector<string> documents;
    documents = qProcessor->runQuery(query);
//    queryResults = qProcessor.runQuery(query);

    //TODO clean this up
//    for (Document doc: queryResults)
//        documents.push_back(doc.title);

    //output query results to user
    cout << "Number of documents: " << documents.size() << '\n' << endl;

    int pageNum = 1;
    while(true) {
        paginateResultingDocuments(documents, pageNum);

        //check to see if user wants to see more documents
        cout    << "View next page? (if no, type \"new command\" to submit a new command)\n"
                << ">>>" << flush;
        string choice;
        getline(cin, choice);
        cout << endl;
        lowercase(choice);

        //user wants to ask a new command
        if (choice == "new command") {
            cout << "REDIRECTING...\n" << endl;
            return;
        }

        pageNum++;
    }
}

void UserInterface::paginateResultingDocuments(vector<string> &documents, int pageNum) {
    cout << "Page " << pageNum << endl;
    for (int i = (pageNum - 1) * 15; i < documents.size() && i < pageNum * 15; i++) {
        cout << i+1 << ". " << documents[i] << endl;
    }
    cout << endl;
}

void UserInterface::getStatistics()
{
    cout << "Number of documents parsed: " << numDocsParsed << endl;
    cout << "Time to parse: " << parseTime << " seconds" << endl;
    cout << "Number of unique words: " << handler.getNumUniqueWords() << endl;
    cout << endl;
}


string UserInterface::lowercase(string command)
{
    string result;
    for (auto iter = command.begin(); iter != command.end(); iter++)
        result += tolower(*iter);

    return result;
}
