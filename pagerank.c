#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "readData.h"

static char *mystrdup(char *string);

int main(int argc, char *argv[]) {
    
    double d = 0;
    double diffPR = 0;
    int maxIters = 0;
    char *dir;
    
    if (argc != 5) {
        
        printf("Usage: %s dampingFactor maxdiff maxiters dir\n", argv[0]);
        exit(1);
        
    } else {
        
        sscanf(argv[1], "%lf", &d);
        sscanf(argv[2], "%lf", &diffPR);
        maxIters = atoi(argv[3]);
        dir = argv[4];
        
    }
    
    char collectionstxt[BUFSIZ] = {0};
    strcat(collectionstxt, dir);
    strcat(collectionstxt, "/");
    strcat(collectionstxt, "collection.txt");
    
    FILE *collection = fopen(collectionstxt, "r");
    
    if (collection == NULL) {
        printf("error opening file\n");
        exit(1);
    }
    
    // Array of pointers to strings (each string is a URL)
    char *urls[BUFSIZ] = {0};
    
    char buffer[BUFSIZ] = {0};
    
    int numUrls = 0;
    
    while(fscanf(collection, "%s", buffer) == 1) {
        
        urls[numUrls] = mystrdup(buffer);
        
        // Effectively reseting the buffer
        buffer[0] = '\0';
        numUrls++;
        
    }
    
    // Just to make sure    
    urls[numUrls] = NULL;
    
    Graph g = graphCreate(numUrls);
    
    // Add urls to the graph
    
    int i = 0;
    
    while (i < numUrls) {
        
        graphAddVertex(g, urls[i]);
        i++;
        
    }
    
    // Read each url and 
    
    // Reset buffer
    buffer[0] = '\0'; 
    
    i = 0;
    
    while (i < numUrls) {
        
        char currentUrl[BUFSIZ] = {0};
        strcat(currentUrl, dir);
        strcat(currentUrl, "/");
        strcat(currentUrl, urls[i]);
        strcat(currentUrl, ".txt");
        
        FILE *currentFile = fopen(currentUrl, "r");
        
        if (currentFile == NULL) {
            
            printf("Couldnt open\n");
            exit(1);
            
        }
        
        printf("%s\n", currentUrl);
        
        // Get rid of the #start section-1
        fscanf(currentFile, "%s", buffer);
        printf("%s\n", buffer);
        fscanf(currentFile, "%s", buffer);
        printf("%s\n", buffer);
                
        while (fscanf(currentFile, "%s", buffer) == 1 && strcmp(buffer, "#end") != 0) {

            // Link the two
            graphConnectVertices(g, urls[i], buffer);
            buffer[0] = '\0';
            
        }
        
        i++;
        
    }
    
    i = 0;
    
    while (i < numUrls) {
        
        
        
        i++;
        
    }
    
    graphPrint(g);
    
    graphFree(g);
    
}

static char *mystrdup(char *string) {
    
    assert(string != NULL);
    
    int len = strlen(string);
    
    // +1 for nul-terminator
    char *retString = calloc(len + 1, sizeof(char));
    
    assert(retString != NULL);
    
    strcat(retString, string);
    
    return retString;
    
}
