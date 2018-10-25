/*
 * Item.c
 * Github: https://github.com/martejj/SearchEngine/Item.c
 * Author: John Shepherd
 * Year: 2013
 * An implementation of an item for a binary tree ADT and sorted list ADT
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "Item.h"

char* ItemCopy(char* i) {
	if (i == NULL) return NULL;
	char *cp = (char*) malloc((strlen(i)+1)*sizeof(char));
	strcpy(cp, i);
	return cp;
}
