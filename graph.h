
#ifndef GRAPH_H
#define GRAPH_H

typedef struct _graph *Graph;

/*
 * Returns a new Graph pointer with memory allocated
 */

Graph graphCreate(int size);

/*
 * Frees all memory assosiated with a given Graph
 */

void graphFree(Graph g);

/*
 * Adds a place in the vertex adjacency list for this key
 */

void graphAddVertex(Graph g, char *key);

/*
 * Adds a directional connection from src to dst on the graph g
 * Assumes src exists
 */

void graphConnectVertices(Graph g, char *src,  char *dst);

/*
 * Returns TRUE if there is an edge between the vertex with keys src and dst
 */

int graphConnectionExists(Graph g, char *src,  char *dst);

/*
 * Returns TRUE if the graph contains the key key
 */

int graphKeyExists(Graph g, char *key);

/*
 * Prints the graph
 */

void graphPrint(Graph g);

/*
 * Simple set of tests to check that the graph is working
 */

void graphTest();

#endif