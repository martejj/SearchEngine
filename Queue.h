#ifndef QUEUE_H
#define QUEUE_H

/*
 * Queue.h
 * Github: https://github.com/martejj/SearchEngine/Queue.h
 * Author: John Shepherd
 * Year: 2013
 * An interface of a Queue ADT
 */

#include "Item.h"

typedef struct QueueRep *Queue;

Queue newQueue(); // create new empty queue
void dropQueue(Queue); // free memory used by queue
void showQueue(Queue); // display as 3 > 5 > 4 > ...
void QueueJoin(Queue,Item); // add item on queue
Item QueueLeave(Queue); // remove item from queue
int QueueIsEmpty(Queue); // check for no items

#endif
