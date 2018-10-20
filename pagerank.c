#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "readData.h"

Graph createURLGraph(char *dir);

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
    
    Graph g = createURLGraph(dir);
    
    graphPrint(g);
    
    graphFree(g);
    
}

Graph createURLGraph(char *dir) {

    char collectionFileName[BUFSIZ] = {0};
    char directoryBase[BUFSIZ] = {0};
    
    if (dir == NULL) {
        
        strcat(collectionFileName, "collection.txt");
        
    } else {
        
        strcat(directoryBase, dir);
        strcat(directoryBase, "/");
        
        strcat(collectionFileName, dir);
        strcat(collectionFileName, "/");
        strcat(collectionFileName, "collection.txt");
        
    }
    
    List urls = getListOfWords(collectionFileName);
    
    Graph graph = graphCreate(urls->nNodes);
    
    ListNode currURL = urls->head;
    
    while (currURL != NULL) {
        
        graphAddVertex(graph, curr->data);
        curr = curr->next;
        
    }
    
    currURL = urls->head;

    while (currURL != NULL) {
        
        char urlFileName[BUFSIZ] = {0};
        strcat(urlFileName, directoryBase);
        strcat(urlFileName, currURL->data);
        strcat(urlFileName, ".txt");
        
        List outURLS = getListOfSec1FromFile(urlFileName);
        
        ListNode currOutURL = outURLS->head;
        
        while (currOutURL != NULL) {
            
            graphConnectVertices(currURL, currOutURL);
            
            currOutURL = currOutURL->next;
            
        }
        
        currURL = currURL->next;
        
    }
    
    return graph;
    
}

