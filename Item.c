// Item.c  ... definition for items in Queues
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "Item.h"

char* ItemCopy(char* i) {
	if (i == NULL) return NULL;
	char *cp = (char*) malloc((strlen(i)+1)*sizeof(char));
	strcpy(cp, i);
	return cp;
}
