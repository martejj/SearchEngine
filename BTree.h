#ifndef BTREE_H
#define BTREE_H

#include "SortedList.h"

typedef struct BTree {
    Item word;
    SortedListPtr urls;
    struct BTree* left;
    struct BTree* right;
} BTree;

typedef struct BTree* BTreePtr;

/*
 * Utility function used for debugging purpose.
 * It displays this binary tree contains.
 */
void BTprint(BTreePtr root);

/*
 * Add list of urls to a node with data 'word'
 */
void BTaddNodeUrls(BTreePtr* root, Item item, SortedListPtr urls);

/*
 * Add an url to a node with data 'word'
 */
void BTaddNode(BTreePtr* root, Item word, Item url);

/*
 * Free all memory allocated for this BTree
 */
void BTfree(BTreePtr root);

/*
 * Return list of urls associated with 'word'
 */
SortedListPtr BTgetNodeURLs(BTreePtr t, Item word);
#endif
