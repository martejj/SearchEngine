#include "readData.h"
#include "Item.h"
#include "SortedList.h"
#include "BTree.h"
#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/*
 * Read file collection.txt.
 * Returns a list of url file names sorted by alphabetical order.
 */
SortedListPtr getCollection(char* file) {
	FILE* ptr = fopen(file, "r");
	if (ptr == NULL) {
		printf("%s: no such file.", file);
		return NULL;
	}
	SortedListPtr ls = NULL;
	char buf[1000];
	while (fscanf(ptr, "%s", buf) == 1) {
		SLaddByValue(&ls, buf);
	}
	fclose(ptr);
	return ls;
}

/*
 * Read all url files from 'urls' list parameter. Construct a binary
 * tree with each node contains the word and all urls in which it is found.
 */
BTreePtr getBTree(SortedListPtr urls) {
	BTreePtr tree = NULL;
	while (urls != NULL) {
		SortedListPtr outUrls = NULL;
		SortedListPtr words = NULL;
		Item url = urls->val;
		char* fn = concat2(url, ".txt");

		FILE* ptr = fopen(fn, "r");
		if (ptr == NULL) {
			printf("%s: no such file.", url);
			return NULL;
		}

		char buf[1000];
		//skip #start Section-1
		fscanf(ptr, "%*s %*s %s ", buf);

		while (strcmp("#end", buf)) {
			SLaddByValue(&outUrls, buf);
			if (fscanf(ptr, "%s", buf) != 1) {
				break;
			}
		}

		//skip Section-1 #start Section-2
		fscanf(ptr, "%*s %*s %*s %s", buf);
		while (strcmp("#end", buf)) {
			SLaddByValue(&words, trim(buf));
//			SLprint(words);
			if (fscanf(ptr, "%s", buf) != 1) {
				break;
			}
		}
		fclose(ptr);
		SortedListPtr tmp = words;
		while (tmp != NULL) {
			Item word = tmp->val;
			BTaddNode(&tree, word, url);
			tmp = tmp->next;
		}
		SLfree(words);
		urls = urls->next;
	}
//	BTprint(tree);
	return tree;
}

// combining two strings together (from previous assignment)
char* concat2(const char* s1, const char* s2) {
	char *result = NULL;
	if (s1 != NULL && s2 != NULL) {
		result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
		if (result != NULL) {
			strcpy(result, s1);
			strcat(result, s2);
		}
		return result;
	}
	if (s1 != NULL) {
		result = malloc(strlen(s1) + 1); // +1 for the null-terminator
		if (result != NULL) {
			strcpy(result, s1);
			return result;
		}
	}
	if (s2 != NULL) {
		result = malloc(strlen(s2) + 1); // +1 for the null-terminator
		if (result != NULL) {
			strcpy(result, s2);
			return result;
		}
	}
	return result;
}

/*
 * Utility function. It does:
 * .removing leading and trailing spaces.
 * .converting all characters to lowercase,
 * .remove the following punctuation marks, if they appear at the end of a word:
 *		'.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
 */
char* trim(char* string) {
	char* tmp = string;
	//trim leading
	while (tmp != NULL) {
		if (tmp[0] == '.' || tmp[0] == ',' || tmp[0] == ';' || tmp[0] == '?') {
			tmp++;
		} else {
			break;
		}
	}
	//trim trailing
	while (strlen(tmp) > 0) {
		if (tmp[strlen(tmp) - 1] == '.' || tmp[strlen(tmp) - 1] == ','
				|| tmp[strlen(tmp) - 1] == ';' || tmp[strlen(tmp) - 1] == '?') {
			tmp[strlen(tmp) - 1] = '\0';
		} else {
			break;
		}
	}
    // convert to lower cases
	int i = 0;
	while (i < strlen(tmp)) {
		tmp[i] = tolower(tmp[i]);
		i++;
	}
	return tmp;
}
