/*
 * scaledFootrule.c
 * Github: https://github.com/martejj/SearchEngine/scaledFootrule.c
 * Author: Harrison Steyn
 * Year: 2018
 * A implementation of Scaled Footrule Rank Aggregation
 * From Specification: 
 * Combines search results (ranks) from multiple sources (say from tf-idf
 * and pagerank) using "Scaled Footrule Rank Aggregation".
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "list.h"
#include "readData.h"

double calculateGuess(List allUrls, List fileText[], int numFiles, int guesses[]);

double scaledFootruleDistance(int oldPos, int oldSize, int newPos, int newSize);

void permute(int data[], int start, int maxIndex, List allUrls, 
    List rankings[], int numRankings, double *bestGuessValue, int bestGuess[]);

int main(int argc, char *argv[]) {
    
    // 1 less as it counts index 0 which isnt a file
    int numFiles = argc - 1;
    
    // We need at least two files
    if (numFiles < 2) {
        
        printf("Usage: %s file1 file2 ... filen\n", argv[0]);
        exit(1);
        
    }
    
    List fileText[numFiles];
    
    List allUrls = listCreate();
    
    // Start at 1 as argv[0] contains ./name
    int i = 1;
    
    // Loop over every file and open it, populating the files array
    while (i < argc) {
        
        // Minus one as i starts at 1
        fileText[i - 1] = getListOfWordsFromFile(argv[i]);
        
        listUnion(allUrls, fileText[i - 1]);
        
        i++;

    }

    int guesses[allUrls->nNodes];

    int bestGuess[allUrls->nNodes];
    memset(bestGuess, 0, allUrls->nNodes*sizeof(int));
    
    double bestGuessValue = 99999999;
    
    // The guesses array needs to contain all numbers possible
    for (i = 0; i < allUrls->nNodes; i++) guesses[i] = i + 1;
    
    // Use the permute function to calculate the best guess
    permute(guesses, 0, allUrls->nNodes - 1,
                allUrls, fileText, numFiles, &bestGuessValue, bestGuess);
    
    printf("%.6f\n", bestGuessValue);
    
    // The index we are currently looking for
    int currSearchPos = 1;
    
    // loop over every position we need to find the url for
    while (currSearchPos < allUrls->nNodes + 1) {
        
        int i = 0;
        
        // Loop over every url to see if its guessed position is the one
        // we are up to
        while (i < allUrls->nNodes) {
            
            if (bestGuess[i] == currSearchPos) {
                
                printf("%s\n", listGetFromIndex(allUrls, i));
                // Break as we found it
                break;
            
            }
            
            i++;
            
        }
        currSearchPos++;
        
    }
    
    // Free memory
    
    i = 0;
    
    while (i < numFiles) {
        
        listFree(fileText[i]);
        
        i++;
        
    }
    
    listFree(allUrls);
    
}

/*
 * Swaps the values of two pointers
 */
 
void swap(int *x, int *y) {
    
    int temp = *x;
    *x = *y;
    *y = temp;
    
}

/*
 * Adapted from: 
 * https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
 * - data contains the sequence to permute
 * - start is where we are up to in permuting it (recursive)
 * - maxIndex is the maximum index in data
 * - the rest is for calculating the guess value
 * - brute force approach
 */

void permute(int data[], int start, int maxIndex, List allUrls, 
    List rankings[], int numRankings, double *bestGuessValue, int bestGuess[]) {
    
    // If we have completed this permutation
    if (start == maxIndex) {
        
        // Calculate the value for this guess
        double guess = calculateGuess(allUrls, rankings, numRankings, data);
        
        // If this guess is better than the current best
        if (guess < *bestGuessValue) {
            
            // Set the guess and copy in this sequence
            *bestGuessValue = guess;
            memcpy(bestGuess, data, (maxIndex + 1)*sizeof(int));
            
        }
        
    } else {
    
        int i = start;
        
        // Loop over every index swapping and then unswapping
        // in order to get different permutations
        while (i <= maxIndex) {
            
            swap(data + start, data + i);
            // Recursively call function but with start one further
            permute(data, start + 1, maxIndex,
                allUrls, rankings, numRankings, bestGuessValue, bestGuess);
            // Undo the swap
            swap(data + start, data + i);
            
            i++;
            
        }
    
    }
    
}

/*
 * Returns the score for a given guess[]. the value of the guess array
 * corresponds to the position in the allUrls and the index the new position
 * e.g. int guesses[] = {1, 3, 2, 5, 4}
 * allUrls(0) is guessed to be in position 1,
 * allUrls(1) is guessed to be in position 3,
 * ...
 * allUrls contains every url being considered from all of the 
 * rankings and rankings is an array of pointers to Lists which
 * each has the urls in their rank order
 */

double calculateGuess(List allUrls, List rankings[], int numRankings, int guesses[]) {
    
    int newSize = allUrls->nNodes, currResultIndex = 0;
    
    double summation = 0;
    
    // Loop over each different ranking system
    while (currResultIndex < numRankings) {
        
        List currRank = rankings[currResultIndex];
        ListNode currEntry = currRank->head;
        int pos = 0;
        
        // Loop over every ordered ranking in the ranking system
        while(currEntry != NULL) {
            
            // As allUrls is the union of every Rank list we assume this is a valid index
            int newPos = guesses[listIndexOf(allUrls, currEntry->data)];
            
            // pos + 1 as we are after is position not index
            summation += scaledFootruleDistance(pos + 1, currRank->nNodes, newPos, newSize);
            
            currEntry = currEntry->next;
            pos++;
            
        }
        
        currResultIndex++;
        
    }
    
    return summation;
    
}

/*
 * Returns the scaled footrule distane from a ranking list at pos oldPos 
 * which has cardinality oldSize to a new rank newPos in list with cardinality
 * newSize
 */

double scaledFootruleDistance(int oldPos, int oldSize, int newPos, int newSize) {
    // printf("%d/%d - %d/%d\n", oldPos, oldSize, newPos, newSize);
    return fabs((1.0*oldPos)/oldSize - (1.0*newPos)/newSize);
    
}
