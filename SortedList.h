#ifndef SORTEDLIST_H
#define SORTEDLIST_H

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
