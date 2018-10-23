// Item.h  ... definition for items in Queues
// Written by John Shepherd, March 2013

#ifndef ITEM_H
#define ITEM_H

#include <string.h>

typedef char* Item;

#define ItemEQ(i1,i2)   (strcmp(i1,i2) == 0)
#define ItemShow(i)     printf("%s",(i))

inline char* ItemCopy(char* i);

#endif

