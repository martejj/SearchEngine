#ifndef READDATA_H
#define READDATA_H

#include "SortedList.h"
#include "BTree.h"

#define BLANK_TOKENS " \t\r\n\v\f"
#define COMMA_BLANK_TOKENS " ,\t\r\n\v\f"

/*
 * Read collection file.
 * Return list of url files in ascending alphabetical order
 */
SortedListPtr getCollection(char* file);

/*
 * Read all url files from 'urls' list parameter. Construct a binary
 * tree with each node contains the word and all urls in which it is found.
 */
BTreePtr getBTree(SortedListPtr urls);

/*
 * Utility function, return concatenation result of two string
 */
char* concat2(const char* s1, const char* s2);

/*
 * Utility function. It does:
 * .removing leading and trailing spaces.
 * .converting all characters to lowercase,
 * .remove the following punctuation marks, if they appear at the end of a word:
 *		'.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
 */
char* trim(char* string);

#endif
