
#ifndef LIST_H
#define LIST_H

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
 * Prints a given list to the specified file
 */

void listPrint(List list, FILE *file);

#endif
