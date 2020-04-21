cmake_minimum_required(VERSION 3.15)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(SearchEngine main.cpp DocumentProcessor.cpp DocumentProcessor.h UserInterface.cpp UserInterface.h stemmer/porter2_stemmer.cpp stemmer/porter2_stemmer.h IndexHandler.cpp IndexHandler.h AVLNode.h AVLTree.h AuthorIndex.h WordIndex.h Word.h Word.cpp HashTable.h HashTable.cpp QueryProcessor.h QueryProcessor.cpp Document.h Document.cpp)