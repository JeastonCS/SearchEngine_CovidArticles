#include "iostream"
#include "Word.h"
#include "AVLTree.h"
using namespace std;
class WordIndex{
private:
    AVLTree<Word> tree;
public:
    void addWord(string word, string doc, double termFreq) {
        Word index = *new Word(word,doc, termFreq);
        AVLNode<Word>* node = tree.find(index);
        if(node == nullptr){
            tree.insert(index);
        } else {
            node->getData().addDocID(doc);
            node->getData().addTermFrequency(termFreq);
        }
    }
    vector<string> getWordDocs(string word){
        Word index(word);
        AVLNode<Word>* node = tree.find(word);
        if(node != nullptr)
            return node->data.getDocIDs();

        vector<string> empty;
        return empty;
    }
    void writeToFile(const char *outFileName) {
        tree.writeToFile(outFileName);
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