#include "iostream"
#include "Word.h"
#include "AVLTree.h"
using namespace std;
class WordIndex{
private:
    AVLTree<Word> tree;
public:
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

    void print() {
        tree.print();
    }
};