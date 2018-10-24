#ifndef URLNODE_H
#define URLNODE_H

#include "Item.h"

typedef struct URLNode {
	int index;
	Item url;
	int score; //number of earch term found in this url
	double rank; //page weight
} URLNode;

typedef struct URLNode* URLNodePtr;

void quickSort1(int intArray[], int left, int right, URLNodePtr nodes);
int compareURLNode(URLNode node1, URLNode node2);
void swap1(int intArray[], int num1, int num2);
int partition1(int intArray[], int left, int right, URLNode pivot,
		URLNodePtr nodes);

#endif
