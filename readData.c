#include "readData.h"
#include "Item.h"
#include "SortedList.h"
#include "BTree.h"
#include "Queue.h"
#include "list.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>

static char *mystrdup(char *string);

static char *concat2(const char *s1, const char *s2);

/*
 * Returns a list of every word in the file (every section of text
 * separated by a space)
 */
 
List getListOfWords(char *fileName) {
	
	FILE *file;
	if ((file = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Error opening file %s : %s\n", fileName, strerror(errno));
		return NULL;
	}
    
    List list = listCreate();
    
    char buffer[BUFSIZ] = {0};
    
    while(fscanf(file, "%s", buffer) == 1) {
        
        // Add the url to the buffer
        listAddToTail(list, buffer);
        
        // Effectively reseting the buffer
        buffer[0] = '\0';
        
    }
    
    fclose(file);
    
    return list;
    
}

/*
 * Read all url files from 'urls' list parameter. Construct a binary
 * tree with each node contains the word and all urls in which it is found.
 */
 
BTreePtr getBTree(List urls) {

	BTreePtr tree = NULL;
	
	ListNode currUrl = urls->head;
	
	while (currUrl != NULL) {
	    
	    List words = listCreate();
	    
	    // Open the URL
	    
		char *url = urls->val;
		char *urlFileName = concat2(url, ".txt");

		FILE *file;
		if ((file = fopen(urlFileName, "r")) == NULL) {
			fprintf(stderr, "Error opening file %s : %s\n", fileName, strerror(errno));
		    return NULL;
		}

		// Skip to #start Section-2

		char buffer[BUFSIZE] = {0};

		while (fscanf(file, "%s", buffer) == 1 && strcmp(buffer, "#end") != 0) {
            
        }

		// Skip Section-1 #start Section-2
		fscanf(ptr, "%*s %*s %*s %s", buf); // TODO
		
		// TODO why add to list just to add list to BTree
		
		while (fscanf(file, "%s", buffer) == 1 && strcmp(buffer, "#end") != 0) {
		
		    // Add the trimmed word to the words list
            listAddToTail(words, trim(buffer));
            
        }
		
		fclose(file);
		
		ListNode currWord = words->head;
		
		while (currWord != NULL) {
		
			BTaddNode(&tree, currWord->data, currUrl->data);
			
			currWord = currWord->next;
			
		}
		
		listFree(words);
		
		currUrl = currUrl->next;
	}
	
//	BTprint(tree);
	return tree;
}

// combining two strings together (from previous assignment)
static char* concat2(const char* s1, const char* s2) {
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

/*
 * As strdup does not exist in c11
 */

static char *mystrdup(char *string) {
    
    assert(string != NULL);
    
    int len = strlen(string);
    
    // +1 for nul-terminator
    char *retString = calloc(len + 1, sizeof(char));
    
    assert(retString != NULL);
    
    strcat(retString, string);
    
    return retString;
    
}
