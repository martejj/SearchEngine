/*
 * URLNode.c
 * Github: https://github.com/martejj/SearchEngine/URLNode.c
 * Author: Grace Nguyen
 * Year: 2018
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "URLNode.h"

void swap1(int intArray[], int num1, int num2) {
	int temp = intArray[num1];
	intArray[num1] = intArray[num2];
	intArray[num2] = temp;
}

// Purpose: sorting using quicksort algorithm

int partition1(int intArray[], int left, int right, URLNode pivot,
		URLNodePtr nodes) {
	int leftPointer = left - 1;
	int rightPointer = right;

	while (1) {
		while (compareURLNode(nodes[intArray[++leftPointer]],pivot) < 0) {
			//do nothing
		}

		while (rightPointer > 0
				&& compareURLNode(nodes[intArray[--rightPointer]], pivot) > 0) {
			//do nothing
		}

		if (leftPointer >= rightPointer) {
			break;
		} else {
			swap1(intArray, leftPointer, rightPointer);
		}
	}
	swap1(intArray, leftPointer, right);
	return leftPointer;
}

void quickSort1(int intArray[], int left, int right, URLNodePtr nodes) {
	if (right - left <= 0) {
		return;
	} else {
		URLNode pivot = nodes[intArray[right]];
		int partitionPoint = partition1(intArray, left, right, pivot, nodes);
		quickSort1(intArray, left, partitionPoint - 1, nodes);
		quickSort1(intArray, partitionPoint + 1, right, nodes);
	}
}


int compareURLNode(URLNode node1, URLNode node2) {
	if (node2.score > node1.score) {
		return 1;
	}
	else if (node2.score < node1.score) {
		return -1;
	}
	if (node2.score == node1.score) {
		if (node2.rank > node1.rank) {
			return 1;
		} else if (node2.rank < node1.rank) {
			return -1;
		}
	}
	return 0;
}
