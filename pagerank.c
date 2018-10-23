#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "list.h"
#include "readData.h"

Graph createURLGraph(char *dir);

double calculateDiff(double *nextPR, double *currPR, int n);

void printIntArray(int *array, int num);
void printDoubleArray(double *array, int num);

void getLinkData(Graph g, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks);

double calculateWin(int v, int u, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks);
double calculateWout(int v, int u, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks);


int main(int argc, char *argv[]) {
    
    //graphTest();
    
    double damp = 0;
    double diffPR = 0;
    int maxIters = 0;
    char *dir;
    
    if (argc != 5) {
        
        printf("Usage: %s dampingFactor maxdiff maxiters dir\n", argv[0]);
        exit(1);
        
    } else {
        
        sscanf(argv[1], "%lf", &damp);
        sscanf(argv[2], "%lf", &diffPR);
        maxIters = atoi(argv[3]);
        dir = argv[4];
        
    }
    
    Graph g = createURLGraph(dir);
    
    double currPR[g->numVertices];
    double nextPR[g->numVertices];
    
    int numInLinks[g->numVertices];
    int numOutLinks[g->numVertices];
    
    int *inLinks[g->numVertices];
    int *outLinks[g->numVertices];
    
    getLinkData(g, numInLinks, numOutLinks, inLinks, outLinks);
    
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        currPR[i] = 1.0/g->numVertices;
        nextPR[i] = 0;
        
        i++;
        
    }
    
    graphTest();
    
    int iteration = 0;
    
    double diff = diffPR;
    
    while (iteration < maxIters && diff >= diffPR) {
        
        printf("%d\n", iteration);
        
        i = 0;
        
        while (i < g->numVertices) {
             
            double summation = 0;
            
            int j = 0;            
            
            while (j < numInLinks[i]) {
                // inLinks[i][j] is the id of the node that connects to i
                
                summation = 
                 (currPR[j]
                *calculateWin(inLinks[i][j], i, numOutLinks, numInLinks, outLinks, inLinks)
                *calculateWout(inLinks[i][j], i, numOutLinks, numInLinks, outLinks, inLinks));
                
                // loop over every node that links to j
                
                j++;
                
            }
            
            nextPR[i] = (1 - damp)/(g->numVertices) + damp*summation;
            
            i++;
            
        }
        
        i = 0;
        
        diff = calculateDiff(nextPR, currPR, g->numVertices);
        
        memcpy(currPR, nextPR, g->numVertices*sizeof(double));
        
        iteration++;
    
    }
    
    printDoubleArray(currPR, g->numVertices);
    
    char outName[BUFSIZ] = {0};
    
    strcat(outName, dir);
    strcat(outName, "/pagerank.txt");
    
    // FILE *file = fopen(outName, "r");
    
    // fclose(file);
    
    graphPrint(g);
    
    graphFree(g);
    
}

double calculateDiff(double *nextPR, double *currPR, int n) {
    
    int i = 0;
    
    double diff = 0;
    
    while (i < n) {
    
        diff += fabs(nextPR[i] - currPR[i]);
    
        i++;
    
    }
    
    return diff;
    
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
    
    List urls = getListOfWordsFromFile(collectionFileName);
    
    Graph graph = graphCreate(urls->nNodes);
    
    ListNode currURL = urls->head;
    
    while (currURL != NULL) {
        
        graphAddVertex(graph, currURL->data);
        currURL = currURL->next;
        
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
            
            graphConnectVertices(graph, graphKeyToID(graph, currURL->data), 
                graphKeyToID(graph, currOutURL->data));
            
            currOutURL = currOutURL->next;
            
        }
        
        currURL = currURL->next;
        
    }
    
    return graph;
    
}

void getLinkData(Graph g, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks) {
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        inLinks[i] = graphGetInlinkIDsFromID(g, i, &numInLinks[i]);
        
        outLinks[i] = graphGetOutlinkIDsFromID(g, i, &numOutLinks[i]);
        
        i++;
        
    }
    
}

double calculateWin(int v, int u, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks) {
    
    int i = 0;
    
    int summation = 0;
    
    while (i < numOutLinks[v]) {
        
        // outLinks[i] is set of pages that v references.
        
        int j = 0;
        
        while (j < numOutLinks[outLinks[v][i]]) {
            
            // Add the number of out links of every page that every page
            // that refers to u refers to.
            summation += numInLinks[j];
            
            j++;
            
        }
        
        i++;
        
    }
    
    if (summation == 0) {
        
        return numInLinks[u]/0.5;
        
    } else {
        
        return numInLinks[u]/(1.0*summation);
    
    }
    
}

double calculateWout(int v, int u, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks) {
    
    // calculated by W = O_u/(sum(0 .. R(v), O_p)
    // R(v) is the set of pages that v references.
    
    int i = 0;
    
    int summation = 0;
    
    while (i < numOutLinks[v]) {
        
        // outLinks[i] is set of pages that v references.
        
        int j = 0;
        
        while (j < numOutLinks[outLinks[v][i]]) {
            
            // Add the number of out links of every page that every page
            // that refers to u refers to.
            summation += numOutLinks[j];
            
            j++;
            
        }
        
        i++;
        
    }
    
    if (summation == 0) {
        
        return numOutLinks[u]/0.5;
        
    } else {
        
        return numOutLinks[u]/(1.0*summation);
    
    }
    
}

void printDoubleArray(double *array, int num) {
    
    int i = 0;
    
    while (i < num) {
        
        printf("%lf ", array[i]);
        
        i++;
        
    }
    
    printf("\n");
    
}

void printIntArray(int *array, int num) {
    
    int i = 0;
    
    while (i < num) {
        
        printf("%d ", array[i]);
        
        i++;
        
    }
    
    printf("\n");
    
}

