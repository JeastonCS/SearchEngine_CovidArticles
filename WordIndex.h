#include "iostream"
using namespace std;
class WordIndex{
private:
//    struct Word{
//        string word;
//        vector<string> documents;
//    };
    AVLTree<Word> tree;
public:
    void addWord(Word& word) {tree.insert(word);}
    Word& getIndex(string name){tree.get(name);}
    void clear(){delete tree;}
};