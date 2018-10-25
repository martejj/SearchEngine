/*
 * list.c
 * Github: https://github.com/martejj/SearchEngine/list.c
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
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
 * Allocates a new node with data data and appends in order in the list
 */

void listAddInOrder(List list, char *data) {
    
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
        
        ListNode curr = list->head;
        ListNode prev = NULL;
        
        while (curr != NULL && strcmp(data, curr->data) >= 0) {
            
            prev = curr;
            curr = curr->next;
            
        }
        
        if (prev != NULL) {
            
            prev->next = newNode;
            newNode->next = curr;
            
        } else {
            
            newNode->next = list->head;
            list->head = newNode;
            
        }
    
    }
    
}

/*
 * Returns a list containing the union of the two lists
 * Assumes sorted.
 */

List listSortedUnion(List list1, List list2) {
    
    List unionList = listCreate();
    
    ListNode curr1 = list1->head;
    ListNode curr2 = list2->head;
    
    while (curr1 != NULL || curr2 != NULL) {
        
        // To deal when one list is longer than the other.
        if (curr1 == NULL) {
            
            listAddToTail(unionList, curr2->data);
            curr2 = curr2->next;
            
        } else if (curr2 == NULL) {
            
            listAddToTail(unionList, curr1->data);
            curr1 = curr1->next;
            
        } else {
            
            // Otherwise we need to find which is smaller
            int cmp = strcmp(curr1->data, curr2->data);
            
            if (cmp == 0) {
                
                listAddToTail(unionList, curr1->data);
                listAddToTail(unionList, curr2->data);
                
                curr1 = curr1->next;
                curr2 = curr2->next;
                
            } else if (cmp < 0) {
            
                listAddToTail(unionList, curr1->data);
                curr1 = curr1->next;
            
            } else {
                
                listAddToTail(unionList, curr2->data);
                curr2 = curr2->next;
                
            }
        
        }
        
    }
    
    return unionList;
    
}

/*
 * Fills list1 with the union of the two lists
 * Does NOT assume sorted.
 */

void listUnion(List list1, List list2) {
        
    ListNode curr2 = list2->head;
    
    while (curr2 != NULL) {
        
        if (!listContains(list1, curr2->data)) {
            
            listAddInOrder(list1, curr2->data);
            
        }
        
        curr2 = curr2->next;
        
    }
    
}

/*
 * Returns a list containing the intersection of the two lists
 * Assumes sorted.
 */

List listSortedIntersection(List list1, List list2) {

    List intersection = listCreate();
    
    ListNode curr1 = list1->head;
    ListNode curr2 = list2->head;
    
    while (curr1 != NULL && curr2 != NULL) {
            
        // Otherwise we need to find which is smaller
        int cmp = strcmp(curr1->data, curr2->data);
        
        if (cmp == 0) {
            
            listAddToTail(intersection, curr1->data);
                            
            curr1 = curr1->next;
            curr2 = curr2->next;
            
        } else if (cmp < 0) {
        
            curr1 = curr1->next;
        
        } else {
            
            curr2 = curr2->next;
            
        }
    
    }
    
    return intersection;

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
 * Returns the index of the node 
 * if it is not contained it returns -1
 */

int listIndexOf(List list, char *data) {
    
    assert(list != NULL);
    assert(data != NULL);
    
    ListNode curr = list->head;
    
    int i = 0;
    
    while (curr != NULL) {
    
        if (strcmp(curr->data, data) == 0) return i;
    
        curr = curr->next;
        i++;
        
    }
    
    return -1;
    
}

/*
 * Returns the index of the node 
 * if it is not contained it returns NULL
 */

char *listGetFromIndex(List list, int index) {
    
    assert(list != NULL);
    
    ListNode curr = list->head;
    
    int i = 0;
    
    while (curr != NULL) {
    
        if (i == index) return curr->data;
    
        curr = curr->next;
        i++;
        
    }
    
    return NULL;
    
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

/*
 * Basic tests for a list
 */

void listTest() {
    
    List list1 = listCreate();
    
    listAddInOrder(list1, "1");
    assert(listContains(list1, "1"));
    
    listAddToHead(list1, "2");
    assert(listContains(list1, "2"));
    
    listFree(list1);
    
    List list2 = listCreate();
    
    listAddInOrder(list2, "1");
    assert(listContains(list2, "1"));
    
    listAddInOrder(list2, "3");
    assert(listContains(list2, "3"));
    
    listAddInOrder(list2, "6");
    assert(listContains(list2, "6"));
    
    List list3 = listCreate();
    
    listAddInOrder(list3, "2");
    assert(listContains(list3, "2"));
    
    listAddInOrder(list3, "3");
    assert(listContains(list3, "3"));
    
    listAddInOrder(list3, "4");
    assert(listContains(list3, "4"));
    
    listAddInOrder(list3, "5");
    assert(listContains(list3, "5"));
    
    listAddInOrder(list3, "6");
    assert(listContains(list3, "6"));
    
    listAddInOrder(list3, "7");
    assert(listContains(list3, "7"));
    
    List unionList = listSortedUnion(list2, list3);
    
    List intersection = listSortedIntersection(list2, list3);
    
    printf("Union of: \n");
    listPrint(list2, stdout);
    
    printf("And: \n");
    listPrint(list3, stdout);
    
    printf("is: \n");
    listPrint(unionList, stdout);
    
    printf("Intersection of: \n");
    listPrint(list2, stdout);
    
    printf("And: \n");
    listPrint(list3, stdout);
    
    printf("is: \n");
    listPrint(intersection, stdout);
}

