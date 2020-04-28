#include "iostream"
#include "Word.h"
#include "AVLTree.h"
using namespace std;
class WordIndex{
private:
    AVLTree<Word> tree;
public:
    void addWord(string word, const Document &doc, double termFreq) {
        Word index = *new Word(word,doc, termFreq);
        AVLNode<Word>* node = tree.find(index);
        if(node == nullptr){
            tree.insert(index);
        } else {
            node->getData().addDoc(doc);
            node->getData().addTermFrequency(termFreq);
        }
    }
    vector<Document> getWordDocs(string word){
        Word index(word);
        AVLNode<Word>* node = tree.find(word);
        if(node != nullptr)
            return node->data.getDocs();

        vector<Document> empty;
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