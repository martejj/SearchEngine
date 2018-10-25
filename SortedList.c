/*
 * SortedList.c
 * Github: https://github.com/martejj/SearchEngine/SortedList.c
 * Author: John Shepherd
 * Year: 2013
 * An implementation of a sorted list for use in BTree.c
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Item.h"
#include "SortedList.h"
#include "Queue.h"

void SLprint(SortedListPtr head) {
	SortedListPtr current = head;
	while (current != NULL) {
		printf("%s >> ", current->val);
		current = current->next;
	}
	printf("END OF LIST\n");
}

int SLisInList(SortedListPtr head, Item item) {
	if (item == NULL) return 0;
	while (head != NULL) {
		if (!strcmp(head->val, item)) {
			return 1;
		}
		head = head->next;
	}
	return 0;
}

void SLaddByValue(SortedListPtr* head, Item item) {
	SortedListPtr current = *head;
	SortedListPtr prev = NULL;
	if (!SLisInList(current, item)) {
		while (current != NULL) {
			if (strcmp(current->val, item) < 0) {
				prev = current;
				current = current->next;
			} else {
				break;
			}
		}

		SortedListPtr tmp = malloc(sizeof(SortedList));
		tmp->next = current;
		tmp->val = ItemCopy(item);
		if (prev != NULL) {
			prev->next = tmp;
		} else {
			*head = tmp;
		}
	}
}

void SLaddByValues(SortedListPtr* head, SortedListPtr items) {
	while (items != NULL) {
		SLaddByValue(head, items->val);
		items = items->next;
	}
}

void SLfree(SortedListPtr head) {
	SortedList *current = head;
	SortedList *next = head;
	while (current) {
		next = current->next;
		free(current->val);
		free(current);
		current = next;
	}
}

int SLsize(SortedListPtr head) {
	int size = 0;
	while (head != NULL) {
		size++;
		head = head->next;
	}
	return size;
}

int SLindexOf(SortedListPtr head, Item item) {
	int index = 0;
	while (head != NULL && strcmp(head->val, item)) {
		index++;
		head = head->next;
	}
	if (head == NULL) {
		return -1; //not found
	}
	return index;
}

Queue SLtoQueue(SortedListPtr head) {
	Queue q = newQueue();
	while (head != NULL) {
		QueueJoin(q, head->val);
		head = head->next;
	}
	return q;
}

Item SLgetByIndex(SortedListPtr head, int index) {
	int i = 0;
	while (head != NULL && i < index) {
		head = head->next;
		i++;
	}
	if (i == index && head != NULL) {
		return ItemCopy(head->val);
	}
	return NULL;
}
