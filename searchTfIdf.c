#include <stdio.h>
#include <stdlib.h>
#include "readData.h"
#include "inverted.h"

/*
 Purpose of this file (copied from the spec):
 In this part, you need to implement a content-based search engine that uses tf-idf
 values of all query terms for ranking. You need to calculate tf-idf values for each
 query term in a page, and rank pages based on the summation of tf-idf values for all
 query terms. Use "inverted index" you created in Part-1 to locate matching pages for
 query terms.
*/

int main(int argc, char *argv[]) {
    // argument processing
    int i = 1;
    SortedListPTr searchTerms = NULL;
    for (/*SortedListPTr searchTerms = NULL*/; i < argc; i++) {
        SLaddByValue(&searchTerms, trim(argv[i]));
    }
    
    // searches for a specific term
    int numberOfSearchTerm = SLsize(searchTerms);
    if (numberOfSearchTerm <= 0) {
        return 0;
    }
    
    // gets the collection.txt file
    SortedListPtr corpus = getCollection("collection.txt");
    int corpusSize = SLsize(corpus);
    
    // ensures that there is a file to be opened
    FILE* ptr = fopen("invertedIndex.txt", "r");
    if (ptr == NULL) {
        printf("%s: no such file.", "invertedIndex.txt");
        return 0;
    }
    
    SortedListPtr* wordToURLs = malloc(numberOfSearchTerm * sizeof(SortedListPtr));
    for (i = 0; i < numberOfSearchTerm; i++) {
        wordToURLs[i] = NULL;
    }
    
    SortedListPtr urls = NULL;
    for(i = 0; i < numberOfSearchTerm; i++) {
        SortedListPtr itr = worldURLs[i];
        while (itr != NULL) {
            Item url = iter->value;
            SLaddByValue(ptr
        }
    }
        
    
    char line[1000];
    
                         
    // term frequency tf(t,d)
    // inverse document frequency idf(t, D)
            
    if (
}
