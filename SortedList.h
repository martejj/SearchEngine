#ifndef SORTEDLIST_H
#define SORTEDLIST_H

/*
 * SortedList.h
 * Github: https://github.com/martejj/SearchEngine/SortedList.h
 * Author: John Shepherd
 * Year: 2013
 * An interface of a sorted list for use in BTree.c
 */

#include "Item.h"
#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct SortedList {
    Item val;
    struct SortedList * next;
} SortedList;


typedef struct SortedList* SortedListPtr;

void SLprint(SortedListPtr head);
int SLisInList(SortedListPtr head, Item item);
void SLaddByValue(SortedListPtr* head, Item item);
void SLaddByValues(SortedListPtr* head, SortedListPtr items);
void SLfree(SortedListPtr head);
Queue SLtoQueue(SortedListPtr head);
int SLsize(SortedListPtr head);
int SLindexOf(SortedListPtr head, Item item);
Item SLgetByIndex(SortedListPtr head, int index);


#endif
