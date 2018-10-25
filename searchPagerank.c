/*
 * searchPagerank.c
 * Github: https://github.com/martejj/SearchEngine/searchPagerank.c
 * Author: Harrison Steyn
 * Year: 2018
 * An implementation of a search engine that uses inverted indexing and
 * weighted pagerank
 * From Specification: 
 * A simple search engine that given search terms (words) as commandline 
 * arguments, finds pages with one or more search terms and outputs 
 * top 30 pages in descending order of number of search terms found 
 * and then within each group, descending order of Weighted PageRank.
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "readData.h"
#include "list.h"

#define TRUE    1
#define FALSE   0
#define MAX_PRINT 30

typedef struct _entry *Entry;

// Stores a search entry
struct _entry{
    
    char *url;
    int nRef;
    double pr;
    
};

void addEntrySightingSorted(Entry entries, char *url, int nEntries);

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

        listAddInOrder(terms, trim(argv[i]));
        
        i++;
        
    }
    
    // listPrint(terms, stdout);
    
    List inverted = getListOfWordsFromFile("invertedIndex.txt");
    // In form "term" -> "url" -> ... -> "url" (repeat)
    
    List pagerank = getListOfWordsFromFile("pagerankList.txt");
    // In form "urlxx" -> "numOuts" -> "PR" (repeat)
    
    ListNode currTerm = terms->head;
    ListNode currInverted = inverted->head;
    
    int numUrls = pagerank->nNodes/3;
    
    i = 0;
    
    // And array of sorted entry structs
    struct _entry entries[numUrls];
    
    ListNode curr = pagerank->head;
    
    while (curr != NULL) {
        
        // So we only have to free once 
        // We also trim to remove commas etc
        entries[i].url = trim(curr->data);
        entries[i].nRef = 0;
                
        curr = curr->next;
        // The num outlinks
        
        curr = curr->next;
        // The pagerank
        sscanf(curr->data, "%lf", &entries[i].pr);
        
        curr = curr->next;
        // The next URL name
        
        i++;
        
    }
    
    while (currTerm != NULL && currInverted != NULL) {
        
        // If the word in the invertedIndex.txt is one of the terms 
        // we are looking for (currTerm) then we want to get a list
        // of all the urls that 
        if (strcmp(currTerm->data, currInverted->data) == 0) {
            
            // Progress the pointer along one as we are now looking at
            // the urls that have this word in it
            currInverted = currInverted->next;
            
            while (currInverted != NULL && strstr(currInverted->data, "url") != NULL) {
                // This goes over every url that has a word 
                addEntrySightingSorted(entries, currInverted->data, numUrls);
                
                currInverted = currInverted->next;
                
            }
            
            currTerm = currTerm->next;
            
        }
        
        currInverted = currInverted->next;
        
    }
    
    i = 0;
    
    int numPrinted = 0;
    
    // Print out ordered and only ones that have hits
    // Also only max 30.
    while (i < numUrls && numPrinted < MAX_PRINT) {
        
        if (entries[i].nRef > 0) {
            printf("%s\n", entries[i].url);
            numPrinted++;
        }
        
        i++;
        
    }
    
    // Free lists
    
    listFree(terms);
    listFree(inverted);
    listFree(pagerank);
    
}

/*
 * Adds a new references to a struct _entry into the entry array
 * and keeps it ordered
 */

void addEntrySightingSorted(Entry entries, char *url, int nEntries) {
    
    int i = 0;
    
    while (i < nEntries) {
        
        if (strcmp(url, entries[i].url) == 0) {
         
            entries[i].nRef++;
            
            // Once weve found and incremented the entry loop back to
            // the start to keep everything in order
            while (i > 0) {
                
                // We sort by number of references first then pagerank
                
                // If num references is equal then we check pagerank
                if (entries[i-1].nRef == entries[i].nRef) {
                    
                    // If num references equal we check pagerank of the
                    // entry earlier in list is less, if it is we swap
                    if (entries[i-1].pr < entries[i].pr) {
                        
                        // Swap
                        struct _entry temp = entries[i-1];
                        entries[i-1] = entries[i];
                        entries[i] = temp;
                        
                    } else {
                        // Otherwise we exit
                        break;
                        
                    }
                    
                } else if (entries[i-1].nRef < entries[i].nRef) {
                    
                    // Otherwise we check the nReferences of the earlier
                    // one is less, if so we swap.
                    
                    // Swap
                    struct _entry temp = entries[i-1];
                    entries[i-1] = entries[i];
                    entries[i] = temp;
                    
                } else {
                    
                    // Otherwise we break and return as our job is done
                    break;
                    
                }
                
                i--;
                
            }
            
            // once weve found and reordered we are done
            return;
            
        }
        
        i++;
        
    }
    
}

