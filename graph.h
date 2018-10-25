#ifndef GRAPH_H
#define GRAPH_H

/*
 * graph.h
 * Github: https://github.com/martejj/SearchEngine/graph.h
 * Author: Harrison Steyn
 * Year: 2018
 * An interface for a sparse graph (as it uses list-adjacency representation)
 */


#define NO_KEY      -1

typedef struct _graph *Graph;
typedef struct _list *List;
typedef struct _vertex *Vertex;

struct _graph {
    char **names;
    int maxVertices;
    int numVertices;
    int numEdges;
    List *vertices;
    
};

/*
 * Returns a new Graph pointer with memory allocated
 */

Graph graphCreate(int size);

/*
 * Frees all memory assosiated with a given Graph
 */

void graphFree(Graph g);

/*
 * Creates a vertex for this key and returns the id.
 */

int graphAddVertex(Graph g, char *key);

/*
 * Returns TRUE if a key exists in names
 */ 

int graphKeyExists(Graph g, char *key);

/*
 * Adds a directional connection from src to dst on the graph g
 * Assumes src exists
 */

void graphConnectVertices(Graph g, int src,  int dst);

/*
 * Returns TRUE if there is an edge between the vertex with ids src and dst
 */

int graphConnectionExists(Graph g, int src, int dst);

/*
 * Returns TRUE if the id is being used
 */

int graphIDExists(Graph g, int id);

/*
 * Returns the number of outlinks from a vertex with id id
 */

int graphGetNumOutLinks(Graph g, int id);

/*
 * Returns an array of ints (ids) that point into an id that is *num long
 */

int *graphGetInlinkIDsFromID(Graph g, int id, int *num);

/*
 * Returns an int array of links from an id of *num length
 */

int *graphGetOutlinkIDsFromID(Graph g, int id, int *num);

/*
 * Prints the graph
 */

void graphPrint(Graph g);

/*
 * Returns the assosiated id to a key
 */

int graphKeyToID(Graph g, char *key);

/*
 * Returns the assosiated key to an id.
 */

char *graphIDToKey(Graph g, int id);

/*
 * Returns TRUE if the given ID is valid
 */

int graphIsIDValid(Graph g, int id);

/*
 * Prints a NULL terminated char **
 */

void printTokens(char **tokens);

/*
 * Simple set of tests to check that the graph is working
 */

void graphTest();

#endif
