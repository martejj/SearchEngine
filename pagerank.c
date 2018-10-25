/*
 * pagerank.c
 * Github: https://github.com/martejj/SearchEngine/pagerank.c
 * Author: Harrison Steyn
 * Year: 2018
 * An implementation of weighted pagerank
 * From specification:
 * reads data from a given collection of pages in the file collection.txt 
 * and builds a graph structure using Adjacency List Representation. 
 * Using the weighted pagerank algorithm, calculates Weighted PageRank 
 * for every url in the file collection.txt. In this file, urls are 
 * separated by one or more spaces or/and new line character. 
 * Adds suffix .txt to a url to obtain file name of the corresponding 
 * "web page".
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <errno.h>
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

double *calculatePagerank(Graph g, double damp, double diffPR, int maxIters);

int main(int argc, char *argv[]) {
    
    //graphTest();
    
    double damp = 0;
    double diffPR = 0;
    int maxIters = 0;
    char *dir;
    
    if (argc == 5) {
        
        sscanf(argv[1], "%lf", &damp);
        sscanf(argv[2], "%lf", &diffPR);
        maxIters = atoi(argv[3]);
        dir = argv[4];
        
    } else if (argc == 4) {
        
        sscanf(argv[1], "%lf", &damp);
        sscanf(argv[2], "%lf", &diffPR);
        maxIters = atoi(argv[3]);
        dir = NULL;
    
    } else {
        
        printf("Usage: %s dampingFactor maxdiff maxiters (dir)\n", argv[0]);
        exit(1);
        
    }
    
    Graph g = createURLGraph(dir);
    
    double *PR = calculatePagerank(g, damp, diffPR, maxIters);
    
    char outFileName[BUFSIZ] = {0};


    if (dir == NULL) {
        
        strcat(outFileName, "pagerankList.txt");
        
    } else {
        
        strcat(outFileName, dir);
        strcat(outFileName, "/");
        strcat(outFileName, "pagerankList.txt");
        
    }
    
    FILE *outFile;
    
    if ((outFile = fopen(outFileName, "w")) == NULL) {
		fprintf(stderr, "Error opening file %s : %s\n", outFileName, strerror(errno));
		exit(1);
	}
    
    // Crude method for printing to file output in desc order
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        // Find the current maximum and print it then look for the next
        // maximum
        int j = 0;
        
        int currMaxIndex = 0;
        
        while (j < g->numVertices) {
            
            if (PR[j] > PR[currMaxIndex]) {
                
                currMaxIndex = j;
                
            }
            
            j++;
            
        }
        
        fprintf(outFile, "%s, %d, %.7f\n",
            graphIDToKey(g, currMaxIndex),
            graphGetNumOutLinks(g, currMaxIndex),
            PR[currMaxIndex]);
        
        i++;
        
        PR[currMaxIndex] = 0.0;
        
    }
    
    
    fclose(outFile);
    
    graphFree(g);
    
    free(PR);
    
}

/*
 * Returns an array of doubles which are the pageranks of pages
 * correspondant to their indices
 */

double *calculatePagerank(Graph g, double damp, double diffPR, int maxIters) {
    
    // Initialise linking data
    
    double *currPR = calloc(g->numVertices, sizeof(double));
    double nextPR[g->numVertices];
    
    int numInLinks[g->numVertices];
    int numOutLinks[g->numVertices];
    
    int *inLinks[g->numVertices];
    int *outLinks[g->numVertices];
    
    getLinkData(g, numInLinks, numOutLinks, inLinks, outLinks);
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        // We initialise to 1/N
        currPR[i] = 1.0/g->numVertices;
        nextPR[i] = 0;
        
        i++;
        
    }
    
    // graphPrint(g);
    
    int iteration = 0;
    
    // So that we enter the while loop the first time
    double diff = diffPR;
    
    while (iteration < maxIters && diff >= diffPR) {
        
        i = 0;
        
        // Loop over every vertex in the graph, i
        while (i < g->numVertices) {
             
            double summation = 0;
            
            int j = 0;            
            
            // Loop over every vertex with an outlink to i, inLinks[i][j]
            while (j < numInLinks[i]) {
            
                // inLinks[i][j] is the id of the node that connects to i
                
                // Sum up every inlink's pagerank weight
                summation += 
                 (1.0*currPR[inLinks[i][j]]
                *calculateWin(inLinks[i][j], i, numInLinks, numOutLinks, inLinks, outLinks)
                *calculateWout(inLinks[i][j], i, numInLinks, numOutLinks, inLinks, outLinks));
                
                j++;
                
            }
            
            // Set the next PR to be equal to the aggregation and damp
            nextPR[i] = (1.0 - damp)/(g->numVertices) + damp*summation;
            
            i++;
            
        }
        
        i = 0;
        
        // Calculate the difference so we can see how close we are
        diff = calculateDiff(nextPR, currPR, g->numVertices);
        
        // currPR becomes nextPR for the next loop
        memcpy(currPR, nextPR, g->numVertices*sizeof(double));
        
        iteration++;
    
    }
    
    // Free the allocated links
    
    i = 0;
    
    while (i < g->numVertices) {
        
        free(inLinks[i]);
        free(outLinks[i]);
        
        i++;
        
    }
    
    return currPR;
    
}

/*
 * Returns the difference between the two double arrays
 */

double calculateDiff(double *nextPR, double *currPR, int n) {
    
    int i = 0;
    
    double diff = 0;
    
    while (i < n) {
    
        diff += fabs(nextPR[i] - currPR[i]);
    
        i++;
    
    }
    
    return diff;
    
}

/*
 * Returns a Graph struct populated with the data from dir
 */

Graph createURLGraph(char *dir) {

    // For debugging purposes we allow an optional dir, if it is used 
    // we use the dir given otherwise we use the directory the program
    // is executed in
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
    
    // All of the urls
    List urls = getListOfWordsFromFile(collectionFileName);
    
    // Create memory for this graph
    Graph graph = graphCreate(urls->nNodes);
    
    // Loop over every URL we read from collection.txt and add it to the
    // graph
    ListNode currURL = urls->head;
    
    while (currURL != NULL) {
        
        graphAddVertex(graph, currURL->data);
        currURL = currURL->next;
        
    }
    
    // Loop over every URL and now that they have been added to the
    // graph we can add their connections
    
    currURL = urls->head;

    while (currURL != NULL) {
        
        // Open the file
        char urlFileName[BUFSIZ] = {0};
        strcat(urlFileName, directoryBase);
        strcat(urlFileName, currURL->data);
        strcat(urlFileName, ".txt");
        
        // Reads from the url section
        List outURLS = getListOfSec1FromFile(urlFileName);
        
        // Make an outlink from the currURL to the currOutURL vertices
        // in the graph.
        ListNode currOutURL = outURLS->head;
        
        while (currOutURL != NULL) {
            
            graphConnectVertices(graph, graphKeyToID(graph, currURL->data), 
                graphKeyToID(graph, currOutURL->data));
            
            currOutURL = currOutURL->next;
            
        }
        
        listFree(outURLS);
        
        currURL = currURL->next;
        
    }
    
    listFree(urls);
    
    return graph;
    
}

/*
 * Populates the arrays with linking data
 */
 
void getLinkData(Graph g, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks) {

    int i = 0;
    
    while (i < g->numVertices) {
        
        inLinks[i] = graphGetInlinkIDsFromID(g, i, &numInLinks[i]);
        
        outLinks[i] = graphGetOutlinkIDsFromID(g, i, &numOutLinks[i]);
        
        i++;
        
    }
    
}

/*
 * Returns the weighted input from v to u 
 */

double calculateWin(int v, int u, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks) {
    
    int i = 0, summation = 0;
    
    // Loop over every node that v references to
    while (i < numOutLinks[v]) {
        
        // outLinks[v][i] is the current page we are looking at that
        // i outlinks to

        // Sum the number of inlinks these pages have for the weight
        summation += numInLinks[outLinks[v][i]];
        
        i++;
        
    }
    
    // We return I_u/Sum(I_p), where p is a node that v references
    return numInLinks[u]/(1.0*summation);
    
}

/*
 * Returns the weighted output from v to u 
 */

double calculateWout(int v, int u, int *numInLinks, int *numOutLinks, int **inLinks, int **outLinks) {
    
    // calculated by W = O_u/(sum(p=0 .. R(v), O_p)
    // R(v) is the set of pages that v references.
    
    int i = 0;
    
    double summation = 0;
    
    while (i < numOutLinks[v]) {
        
        // outLinks[i] is set of pages that v references.
        
        // As it must return 0.5 if there are no outlinks
        if (numOutLinks[outLinks[v][i]] == 0) {
            
            summation += 0.5;
            
        } else {
        
            summation += numOutLinks[outLinks[v][i]];
        
        }
        
        i++;
        
    }
    
    // Again it must return 0.5 if there are no outlinks
    return (numOutLinks[u] == 0 ? 0.5 : numOutLinks[u])/(1.0*summation);
    
}

/*
 * For debugging purposes, prints an array of doubles
 */

void printDoubleArray(double *array, int num) {
    
    int i = 0;
    
    while (i < num) {
        
        printf("%lf ", array[i]);
        
        i++;
        
    }
    
    printf("\n");
    
}

/*
 * For debugging purposes, prints an array of ints
 */

void printIntArray(int *array, int num) {
    
    int i = 0;
    
    while (i < num) {
        
        printf("%d ", array[i]);
        
        i++;
        
    }
    
    printf("\n");
    
}

