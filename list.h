#ifndef LIST_H
#define LIST_H

/*
 * list.h
 * Github: https://github.com/martejj/SearchEngine/list.h
 * Author: Harrison Steyn
 * Year: 2018
 * An interface of a list
 */

typedef struct _listNode *ListNode;
typedef struct _list *List;

struct _listNode {

    char *data;
    ListNode next;

};

struct _list {
    
    int nNodes;
    ListNode head;
    ListNode tail;

};

/*
 * Allocates a new node with data data and prepends it to the front of 
 * the list
 */

void listAddToHead(List list, char *data);

/*
 * Allocates a new node with data data and appends it to the end of 
 * the list
 */

void listAddToTail(List list, char *data);

/*
 * Allocates a new node with data data and appends in order in the list
 */

void listAddInOrder(List list, char *data);

/*
 * Returns a list containing the union of the two lists
 * Assumes sorted.
 */

List listSortedUnion(List list1, List list2);

/*
 * Returns a list containing the intersection of the two lists
 * Assumes sorted.
 */

List listSortedIntersection(List list1, List list2);

/*
 * Fills list1 with the union of the two lists
 * Does NOT assume sorted.
 */

void listUnion(List list1, List list2);

/*
 * Allocates a new ListNode 
 */

ListNode listNodeCreate();

/*
 * Frees a allocated ListNode
 */

void listNodeFree(ListNode node);

/*
 * Allocates a new list
 */

List listCreate();

/*
 * Frees a allocated List
 */

void listFree(List list);

/*
 * Returns TRUE if data is contained in a node of list
 */

int listContains(List list, char *data);

/*
 * Returns the index of the node 
 * if it is not contained it returns -1
 */

int listIndexOf(List list, char *data);

/*
 * Returns the index of the node 
 * if it is not contained it returns NULL
 */

char *listGetFromIndex(List list, int index);

/*
 * Prints a given list to the specified file
 */

void listPrint(List list, FILE *file);

/*
 * Basic tests for a list
 */

void listTest();

#endif
