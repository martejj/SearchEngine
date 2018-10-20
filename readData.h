
#ifndef READDATA_H
#define READDATA_H

#define BLANK_TOKENS " \t\r\n\v\f"
#define COMMA_BLANK_TOKENS " ,\t\r\n\v\f"

/*
 * Returns a list of every word in the file (every section of text
 * separated by a space)
 */
 
List getListOfWords(char *fileName);

/*
 * Read all url files from 'urls' list parameter. Construct a binary
 * tree with each node contains the word and all urls in which it is found.
 */
 
BTreePtr getBTree(List urls);

/*
 * Utility function. It does:
 * .removing leading and trailing spaces.
 * .converting all characters to lowercase,
 * .remove the following punctuation marks, if they appear at the end of a word:
 *		'.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
 */

char *trim(char *string);

#endif
