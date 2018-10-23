
#ifndef READDATA_H
#define READDATA_H

#define BLANK_TOKENS " \t\r\n\v\f"
#define COMMA_BLANK_TOKENS " ,\t\r\n\v\f"

#include "list.h"
#include "graph.h"

/*
 * Returns a list of every word in the file (every section of text
 * separated by a space)
 */
 
List getListOfWordsFromFile(char *fileName);

/*
 * Returns a list of every word in section one of the file
 */
 
List getListOfSec1FromFile(char *fileName);

/*
 * Returns a list of every word in section two of the file
 */
 
List getListOfSec2FromFile(char *fileName);

/*
 * Read all url files from 'urls' list parameter. Construct a binary
 * tree with each node contains the word and all urls in which it is found.
 */
 
// BTreePtr getBTree(List urls);

/*
 * Utility function. It does:
 * .removing leading and trailing spaces.
 * .converting all characters to lowercase,
 * .remove the following punctuation marks, if they appear at the end of a word:
 *		'.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
 */

char *trim(char *string);

#endif
