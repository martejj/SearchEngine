#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "readData.h"
#include "list.h"

int main(int argc, char *argv[]) {
    
    //char *dir = "tests/1";
    
    if (argc == 1) {
        
        printf("Usage: %s term term ...\n", argv[0]);
        exit(1);
        
    }
    
    // i starts at 1 as index 0 of argv is file name
    int i = 1;
    
    List terms = listCreate();
    
    while (i < argc) {

        listAddInOrder(terms, argv[i]);
        
        i++;
        
    }
    
    listPrint(terms, stdout);
    
    listTest();
    /*
    List inverted = getListOfWordsFromFile("invertedIndex.txt");
    List pagerank = getListOfWordsFromFile("pagerankList.txt");
    // In form "urlxx" -> "numOuts" -> "PR"
    
    List urls = listCreate();
    
    ListNode currTerm = terms->head;
    ListNode currInverted = inverted->head;
    
    while (currTerm != NULL && currInverted != NULL) {
        
        if (strcmp(currTerm->data, currInverted) == 0) {
            
            
            currTerm = currTerm->next;
            
        }
        
        currInverted = currInverted->next;
        
    } */
    
}
