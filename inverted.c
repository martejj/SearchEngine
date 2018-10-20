#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "readData.h"

/*
 * This out put a binary tree to a file in desired format as specified
 * in assignment requirement.
 */
void printBTtoFile(BTreePtr root, FILE *fptr);

/*
 * This out put a sorted list to a file in desired format as specified
 * in assignment requirement.
 */
void printSLtoFile(SortedListPtr head, FILE *fptr);

/*
 * Reads data from a given collection of pages in collection.txt
 * and generates an "inverted index" that provides a sorted list
 * (set) of urls for every word in a given collection of pages.
 * Before inserting words in your index, you need to "normalise"
 * words by, removing leading and trailing spaces, converting all
 * characters to lowercase, remove the following punctuation marks,
 * if they appear at the end of a word: '.' (dot), ',' (comma),
 * ';' (semicolon), ? (question mark).
 * In each sorted list (set), duplicate urls are not allowed.
 * This program output this "inverted index" to a file named
 * invertedIndex.txt. One line per word, words should be
 * alphabetically ordered, using ascending order.
 * Each list of urls (for a single word) should be alphabetically
 * ordered, using ascending order.
 */

int main(int argc, char *argv[]) {
    List list = NULL;
    //read collection file to get all url data file.
    if (argc >= 2) {
        list = getListOfWords(argv[1]);
    } else {
        list = getListOfWords("collection.txt");
    }
    
    //read each url file and build a binary tree.
    BTreePtr tree = getBTree(list);
    
    FILE *file;
    
    if ((file = fopen("invertedIndex.txt", "w")) == NULL) {
        fprintf(stderr, "Error opening file %s : %s\n", fileName, strerror(errno));
        exit(1);
    }
    
    printBTtoFile(tree, file);
    fclose(file);
    BTfree(tree);
    return 0;
}

/*
 * This out put a binary tree to a file in desired format as specified
 * in assignment requirement.
 */
void printBTtoFile(BTreePtr root, FILE *fptr) {
    if (root == NULL) return;
    printBTtoFile(root->left, fptr);
    fprintf(fptr,"%s ", root->word);
    printSLtoFile(root->urls, fptr);
    printBTtoFile(root->right, fptr);
}



