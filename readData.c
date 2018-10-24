#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include "readData.h"

static char *mystrdup(char *string);

static char *concat2(const char *s1, const char *s2);

/*
 * Returns a list of every word in the file (every section of text
 * separated by a space)
 */
 
List getListOfWordsFromFile(char *fileName) {
	
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
    }
    
    fclose(file);
    
    // To stop complaints
    free(concat2("ds", "ds"));
    free(mystrdup("sadsa"));
    
    return list;
}

/*
 * Returns a list of every word in section one of the file
 */
 
List getListOfSec1FromFile(char *fileName) {
	
	FILE *file;
	if ((file = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Error opening file %s : %s\n", fileName, strerror(errno));
		return NULL;
	}
    
    List list = listCreate();
    
    char buffer[BUFSIZ] = {0};
        
    // Get rid of the #start section-1
    fscanf(file, "%s", buffer);
    fscanf(file, "%s", buffer);
            
    while (fscanf(file, "%s", buffer) == 1 && strcmp(buffer, "#end") != 0) {

        listAddToTail(list, buffer);
        
    }
    
    fclose(file);
    
    return list;
    
}

/*
 * Returns a list of every word in section two of the file
 */
 
List getListOfSec2FromFile(char *fileName) {
	
	FILE *file;
	if ((file = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Error opening file %s : %s\n", fileName, strerror(errno));
		return NULL;
	}
    
    List list = listCreate();
    
	// Skip to #end Section-1

	char buffer[BUFSIZ] = {0};

	while (fscanf(file, "%s", buffer) == 1 && strcmp(buffer, "#end") != 0) {
        
    }

	// Skip Section-1 #start Section-2
	fscanf(file, "%*s %*s %*s %s", buffer); // TODO correct?
	
	while (fscanf(file, "%s", buffer) == 1 && strcmp(buffer, "#end") != 0) {
	
	    // Add the trimmed word to the words list
        listAddToTail(list, trim(buffer));
        
    }
    
    fclose(file);

    return list;
    
}

/*
 * Read all url files from 'urls' list parameter. Construct a binary
 * tree with each node contains the word and all urls in which it is found.
 */
 
BTreePtr getBTree(List urls, char *dir) {

	BTreePtr tree = NULL;
	
	ListNode currUrl = urls->head;
	
	while (currUrl != NULL) {
	    
	    char urlFileName[BUFSIZ] = {0};
	    
	    if (dir == NULL) {
            
            strcat(urlFileName, currUrl->data);
            strcat(urlFileName, ".txt");
            
        } else {
            
            strcat(urlFileName, dir);
            strcat(urlFileName, "/");
            strcat(urlFileName, currUrl->data);
            strcat(urlFileName, ".txt");
            
        }

		List words = getListOfSec2FromFile(urlFileName);
		
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
// May need to consider another structure

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
