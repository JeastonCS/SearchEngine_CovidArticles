# JSON Search Engine

Introduction
============

In this paper we will look at the ability of both an AVL Tree and
Hashtable to manage, store, and retrieve data from inverted indices. The
importance of this task can be seen in any JSON (JavaScript Object Notation) where a search term is
linked to a document, author, and the frequency of the word in the text.

### Considering how data is stored

In this project, the data for the inverted index file for both words and
authors was written out and stored in a text file. In order to do this, the necessary data was
placed in the appropriate data structure through the classes of WordIndex
and AuthorIndex. The word index data is stored by the word then followed by the 
corresponding termFrequency of that document and the docID the word was in.The 
author indexes similarily has the author name and a list of document ID's the
name was found in. Additionally, we used another file to store additional 
details for each Document. Using a vector of Documents to populate to and 
from the file, we are able to extract the publication date, authors, url, 
title, and text of the JSON files. 

By storing to a file rather than reading from a directory of the files multiple 
times, we save a lot of time reading from the files. Instead of reading from 
approxiametely 12000 documents in a corpus with around 1000 to 2000 lines each, after 
storing the necessary data, we only need to read from the three files mentioned above
where each line is efficiently inserted back into the indexes/data structures. 
Furthermore, we used corresponding tree traversal and hashing algorithms to order 
the files in a manner that reduces the complexity of insertions for both data structures.

### Why the experiment is important:

Due to manipulation of the storing of indices, such as keeping the
count, word pages, and documents occurred, the performance will have
even more variance due to these variables. As a node is inserted, the
iterations of maintaining every node has its own vector structure to
store documents. Logically the hashtable will also have a big O of N to
maintain. These deviances require a more empirical approach performance.

### 

### Steps taken:

First we will discuss the different implementation of AVL tree and
hashtable. Then we will take into account the time complexity of each structure 
for corpuses fo different sizes.  We will use an isolated virtual machine, 
only running daemon process and the Search Engine with time command to 
measure the speed. The rate at speed performance will then be gathered and then analyzed.

Data structure overview
-----------------------

## Data being stored

### Introduction to AVL tree implementation:

In the AVL tree each inverted index is a node that contains a Word class which 
stores the document and corresponding term frequency. 

AVL trees are descended from BST which additionally have the capability
of maintaining balance difference max height of two. This reduces the
cost of searching that can occur as a regular BST develops an unbalanced
ratio and drifts away into a LinkedList formated data structure.

#### Searching pseudocode AVL:

Initiate search from root

Using a temp pointer to the index      

    while(node ptr is not at a null leaf) {

      if(the keyword is greater than the current index)

          go down right node

      else if(the keyword is less than the current index)

          go down left node

      else if(keyword is at its index)

          return the documents where that word occurs

    }

   Not found comes here

   

#### Inserting AVL:

Inserting into the AVL is a similar algorithm but instead of returning
in the the documents where that word occurs it either updates causing an
unbalancing and then balancing effect or instead an increase in count if
the that word has been there before. Inserting into a tree of index is
therefore slower than just searching a tree of indexes.

#### Writing to File

When writing to the Word Index file, the AVL tree uses a breadth first traversal 
in order to gurantee the least amount of traversals and node-swapping on insertion.
The breadth first traversal display the contents of the node based on their level, 
not branch. This means that the first node seen and printed is root, the next two are roots branches, and 
next four are those branches and so on.

### Introduction to Hashtable implementation:

The hashtable is a data structure that is similar to the functions of an
array. In the hashtable written for the program, each spot on the table
contained an item object. The item object held inside of it four
different variables: a string to hold the word, a string to hold the PDF
name, an int to hold the number of times it was in each PDF, and a
vector of item objects used to store items with the same key value as
the item. In order to place the item object in a specific index on the
hashtable, a hashing function was used. The purpose of the hashing
function is generate a key value, (a unique number) for each word that
needs to be inserted into the hashtable.  The hashing function generates
this unique number ,and then stores the item object at the index of the
number on the hashtable. In this way, each item object has a unique
location on the hashtable, and that location can be found by running the
word being searching for through the hashing function. For the case of
any collisions (item objects that generate the same number when run
through the hashing function), the colliding item object was added to
the vector of the already existing item object. This way, the hashtable
does not write over previously stored data.

When reading in form the inverted index, the hashtable converts the data
into an item object. It then takes the word, and puts it through the
hashing function, and stores the item at this index on the hashtable.
The bigO of this insertion operation is O(1) or constant time. If the
hashtable goes to insert, and there is already another item present, the
hashtable just adds the colliding item to the end of the vector. This
insertion operation is also O(1).  

When searching for a word, the hashtable just inserts the word into the
hash function. This generates the index of where the word would be
stored on the hashtable if present. Then it just searches all of the
items at that index for that word. The bigO of this operation is
constant time or O(1).



