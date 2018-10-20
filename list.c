#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

#define TRUE    1
#define FALSE   0

static char *mystrdup(char *string);

/*
 * Allocates a new node with data data and prepends it to the front of 
 * the list
 */

void listAddToHead(List list, char *data) {
    
    assert(list != NULL);
    assert(data != NULL);
    
    ListNode newNode = listNodeCreate();
    
    newNode->data = mystrdup(data);
    
    list->nNodes++;
    
    if (list->head == NULL || list->tail == NULL) {
        
        list->head = newNode;
        list->tail = newNode;
        
    } else {
    
        newNode->next = list->head;
        list->head = newNode;
    
    }
    
}

/*
 * Allocates a new node with data data and appends it to the end of 
 * the list
 */

void listAddToTail(List list, char *data) {
    
    assert(list != NULL);
    assert(data != NULL);
    
    ListNode newNode = listNodeCreate();
    
    newNode->data = mystrdup(data);
    newNode->next = NULL;
    
    list->nNodes++;
    
    if (list->head == NULL || list->tail == NULL) {
        
        list->head = newNode;
        list->tail = newNode;
        
    } else {
        
        list->tail->next = newNode;
        list->tail = newNode;
    
    }
    
}

/*
 * Allocates a new ListNode 
 */

ListNode listNodeCreate() {
    
    ListNode node = calloc(1, sizeof(*node));

    assert(node != NULL);
    
    return node;
    
}

/*
 * Frees a allocated ListNode
 */

void listNodeFree(ListNode node) {
    
    assert(node != NULL);
    
    free(node->data);
    
    free(node);    
}

/*
 * Allocates a new list
 */

List listCreate() {
    
    List list = calloc(1, sizeof(*list));
    
    assert(list != NULL);
    
    return list;
    
}

/*
 * Frees a allocated List
 */

void listFree(List list) {
    
    assert(list != NULL);
    
    ListNode curr = list->head;
    ListNode prev = NULL;
        
    while (curr != NULL) {
    
        prev = curr;
        curr = curr->next;    
        free(prev);
        
    }
    
    free(list);
    
}

/*
 * Returns TRUE if data is contained in a node of list
 */

int listContains(List list, char *data) {
    
    assert(list != NULL);
    assert(data != NULL);
    
    ListNode curr = list->head;
        
    while (curr != NULL) {
    
        if (strcmp(curr->data, data) == 0) return TRUE;
    
        curr = curr->next;
        
    }
    
    return FALSE;
    
}

/*
 * Prints a given list to the specified file
 */

void listPrint(List list, FILE *file) {
    
    assert(list != NULL);
    assert(file != NULL);
    
    ListNode curr = list->head;
        
    while (curr != NULL) {
    
        fprintf(file, "%s ", curr->data);
    
        curr = curr->next;
        
    }
    
    fprintf(file, "\n");
    
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

