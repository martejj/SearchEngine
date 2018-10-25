#ifndef ITEM_H
#define ITEM_H

/*
 * Item.h
 * Github: https://github.com/martejj/SearchEngine/Item.h
 * Author: John Shepherd
 * Year: 2013
 * An interface of an item for a binary tree ADT and sorted list ADT
 */

#include <string.h>

typedef char* Item;

#define ItemEQ(i1,i2)   (strcmp(i1,i2) == 0)
#define ItemShow(i)     printf("%s",(i))

char* ItemCopy(char* i);

#endif

