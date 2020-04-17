#include "iostream"
#include "Word.h"
#include "AVLTree.h"
using namespace std;
class WordIndex{
private:
    AVLTree<Word> tree;
public:
    void addWord(string word, string doc) {
        Word index(word,doc);
        AVLNode<Word>* node = tree.find(index);
        if(node == nullptr){
            tree.insert(index);
        } else {
            vector<string> docs = node->data.getDocIDs();
            docs.push_back(doc);
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
    void clear(){
        tree.clear();
    }

    int getNumUniqueWords() {
        return tree.getNumNodes();
    }
};