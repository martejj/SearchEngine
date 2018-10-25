/*
 * BTree.c
 * Github: https://github.com/martejj/SearchEngine/BTree.c
 * Author: John Shepherd
 * Year: 2013
 * An implementation of a binary tree ADT
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Item.h"
#include "BTree.h"

/*
 * Utility function used for debugging purpose.
 * It displays this binary tree contains.
 */
void BTprint(BTreePtr root) {
	if (root == NULL) return;
	BTprint(root->left);
	printf("###WORD### %s\n", root->word);
	printf("###URLS###\n");
	SLprint(root->urls);
	BTprint(root->right);
}


/*
 * Add list of urls to a node with data 'word'
 */
void BTaddNodeUrls(BTreePtr* root, Item word, SortedListPtr urls) {
	while (urls != NULL) {
		BTaddNode(root, word, urls->val);
		urls = urls->next;
	}
}

/*
 * Add an url to a node with data 'word'
 */
void BTaddNode(BTreePtr* t, Item word, Item url) {
    if (*t == NULL) {
        *t = malloc(sizeof(struct BTree));
        (*t)->left = NULL;
        (*t)->right = NULL;
        (*t)->urls = NULL;
        (*t)->word = ItemCopy(word);
        if (url != NULL) {
        	SLaddByValue(&((*t)->urls), url);
        }
    } else if (!strcmp((*t)->word, word)) {
    	if (url != NULL) {
    		SLaddByValue(&((*t)->urls), url);
    	}
    } else if (strcmp((*t)->word, word) > 0) {
        BTaddNode(&((*t)->left), word, url);
    } else {
        BTaddNode(&((*t)->right), word, url);
    }
}

/*
 * Return list of urls associated with 'word'
 */
SortedListPtr BTgetNodeURLs(BTreePtr t, Item word) {
	if (t != NULL) {
		if (!strcmp(t->word, word)) {
			return t->urls;
		} else if (strcmp(t->word, word) > 0) {
			return BTgetNodeURLs(t->left, word);
		} else {
			return BTgetNodeURLs(t->right, word);
		}
	}
	return NULL;
}

/*
 * Free all memory allocated for this BTree
 */
void BTfree(BTreePtr root) {
	if (root == NULL) return;
	BTfree(root->left);
	free(root->word);
	SLfree(root->urls);
	BTfree(root->right);
}

