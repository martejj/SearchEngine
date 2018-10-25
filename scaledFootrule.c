/*
 * scaledFootrule.c
 * Github: https://github.com/martejj/SearchEngine/scaledFootrule.c
 * 
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
    
    int guesses[] = {1, 4, 2, 5, 3};
    
    double guess = calculateGuess(allUrls, fileText, numFiles, guesses);
    
    i = 0;
    
    printf("%lf\n", guess);
    
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
    
    while (currResultIndex < numRankings) {
        
        List currRank = rankings[currResultIndex];
        ListNode currEntry = currRank->head;
        int pos = 0;
        
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
