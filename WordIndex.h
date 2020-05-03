#include "iostream"
#include "Word.h"
#include "AVLTree.h"
using namespace std;
class WordIndex{
private:
    AVLTree<Word> tree;
public:
    WordIndex &operator=(const WordIndex &rhs) {
        tree = rhs.tree;
        return *this;
    }
    void addWord(string word, const string &docID, double termFreq) {
        Word index = *new Word(word,docID, termFreq);
        AVLNode<Word>* node = tree.find(index);
        if(node == nullptr){
            tree.insert(index);
        } else {
            node->getData().addDocID(docID);
            node->getData().addTermFrequency(termFreq);
        }
    }
    vector<string> getWordDocIDs(string word){
        Word index(word);
        AVLNode<Word>* node = tree.find(word);
        if(node != nullptr)
            return node->data.getDocIDs();

        vector<string> empty;
        return empty;
    }

    vector<double> getTermFreq(string word) {
        Word index(word);
        AVLNode<Word>* node = tree.find(word);
        if(node != nullptr)
            return node->data.getTermFrequencies();

        vector<double> empty;
        return empty;
    }

    void writeToFile(const char *fileName) {
        tree.writeToFile(fileName);
    }

    void clear(){
        tree.clear();
    }

    int getNumUniqueWords() {
        return tree.getNumNodes();
    }

    vector<Word> getTopFifty() {
        vector<Word> topFiftyWords;
        saveTopFifty(topFiftyWords, tree.getRoot());

        return topFiftyWords;
    }

    void saveTopFifty(vector<Word> &topFiftyWords, AVLNode<Word> *curr)
    {
        if (curr != nullptr) {
            //left sub-tree
            saveTopFifty(topFiftyWords, curr->left);

            //add to vector if big enough
            if (topFiftyWords.size() == 50) { //erases min value and inserts new value
                //look to see where it fits into vector
                vector<Word>::iterator it;
                for (it = topFiftyWords.begin(); it != topFiftyWords.end(); it++) {
                    if ( it->getNumDocs() > curr->getData().getNumDocs() ) {
                        topFiftyWords.insert(it, curr->getData());
                        topFiftyWords.erase(topFiftyWords.begin());
                        break;
                    }
                }
                //curr data is max of vector in current state
                if (it == topFiftyWords.end()) {
                    topFiftyWords.erase( topFiftyWords.begin() );
                    topFiftyWords.push_back( curr->getData() );
                }
            }
            else {
                //look to see where it fits into vector
                vector<Word>::iterator it;
                for (it = topFiftyWords.begin(); it != topFiftyWords.end(); it++) {
                    if ( it->getNumDocs() > curr->getData().getNumDocs() ) {
                        topFiftyWords.insert(it, curr->getData());
                        break;
                    }
                }
                //curr data is max of vector in current state
                if (it == topFiftyWords.end()) {
                    topFiftyWords.push_back( curr->getData() );
                }
            }

            //right sub-tree
            saveTopFifty(topFiftyWords, curr->right);
        }
    }

    void print() {
        tree.print();
    }
};