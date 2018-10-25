/*
 * inverted.c
 * Github: https://github.com/martejj/SearchEngine/inverted.c
 * Author: Grace Nguyen & Harrison Steyn
 * Year: 2018
 * An implementation of an inverted index calculator
 * From specification:
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "readData.h"
#include "BTree.h"
#include "SortedList.h"
#include "Queue.h"

void printBTtoFile(BTreePtr root, FILE *fptr);

void printSLtoFile(SortedListPtr head, FILE *fptr);


int main(int argc, char *argv[]) {

    List list = NULL;
    
    char *dir = NULL;
    
    //read collection file to get all url data file.
    if (argc >= 2) {
    
        char fileName[BUFSIZ] = {0};
        strcat(fileName, argv[1]);
        strcat(fileName, "/collection.txt");
        list = getListOfWordsFromFile(fileName);
        
        dir = argv[1];
        
    } else {
    
        list = getListOfWordsFromFile("collection.txt");
    }
    
    //read each url file and build a binary tree.
    BTreePtr tree = getBTree(list, dir);
    
    // For debugging
    
    char fileName[BUFSIZ] = {0};
    
    if (dir == NULL) {
        
        strcat(fileName, "invertedIndex.txt");
        
    } else {
        
        strcat(fileName, dir);
        strcat(fileName, "/invertedIndex.txt");
        
    }
    
    FILE *file;
    
    if ((file = fopen(fileName, "w")) == NULL) {
        fprintf(stderr, "Error opening file %s : %s\n", fileName, strerror(errno));
        exit(1);
    }
    
    printBTtoFile(tree, file);
    fclose(file);
    BTfree(tree);
    listFree(list);
    
    return 0;
}

/*
 * This out put a binary tree to a file in desired format as specified
 * in assignment requirement.
 */
void printBTtoFile(BTreePtr root, FILE *fptr) {
    if (root == NULL) return;
    printBTtoFile(root->left, fptr);
    fprintf(fptr,"%s  ", root->word);
    printSLtoFile(root->urls, fptr);
    printBTtoFile(root->right, fptr);
}

/*
 * This out put a sorted list to a file in desired format as specified
 * in assignment requirement.
 */
void printSLtoFile(SortedListPtr head, FILE *fptr) {
    SortedListPtr current = head;
    while (current != NULL) {
        fprintf(fptr,"%s ", current->val);
        current = current->next;
    }
    fprintf(fptr,"%s", "\n");
}


