#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "graph.h"

#define TRUE    1
#define FALSE   0

// A  list contains all out links of the key
// use adjacency list representation as this is a sparce graph
struct _list {
    
    int id;
    int numVertices;
    Vertex head;
    Vertex last;
    
};

struct _vertex {
    
    int id;
    Vertex next;
    
};
 
static Vertex newVertex(int id);
static List newVertexList(int id);

static void listFree(List list);

static void appendVertex(List list, Vertex vertex);

static int getNumVerticesIn(Graph g, int id);

static int listContains(List list, int id);

void printTokens(char **tokens);

static char *mystrdup(char *string);

/*
 * Returns a new Graph pointer with memory allocated
 */

Graph graphCreate(int size) {
    
    Graph g = calloc(1, sizeof(*g));
    
    assert(g != NULL);
    
    g->maxVertices = size;
    
    // Sizeof list (the pointer) as it is an array of pointers
    g->vertices = calloc(size, sizeof(List));
    
    g->names = calloc(size, sizeof(char *));
    
    assert(g->vertices != NULL);
    
    return g;
    
}

/*
 * Returns a new vertex with the id given
 */
 
static Vertex newVertex(int id) {
    
    Vertex new = calloc(1, sizeof(*new));
    
    assert(new != NULL);
    
    new->id = id;
    
    return new;
    
}

/*
 * Returns a new list with the id given
 */

static List newVertexList(int id) {
    
    List new = calloc(1, sizeof(*new));
    
    assert(new != NULL);
    
    new->id = id;
    
    return new;
    
}

/*
 * Frees all memory assosiated with a given Graph
 */

void graphFree(Graph g) {
    
    assert(g != NULL);
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        listFree(g->vertices[i]);
        
        i++;
    }
    
    free(g->vertices);
    free(g->names);
    free(g);
        
}

/*
 * Frees an entire list
 */

static void listFree(List list) {

    assert(list != NULL);
    
    Vertex curr = list->head;
    Vertex prev = NULL;
    
    while (curr != NULL) {
        
        prev = curr;
        curr = curr->next;
        
        free(prev);
        
    }
    
    free(list);
    
}

/*
 * Creates a vertex for this key and returns the id.
 */

int graphAddVertex(Graph g, char *key) {
    
    assert(g != NULL);
    assert(key != NULL);
    
    // So that we dont have two entries for the same key
    if (graphKeyExists(g, key)) return -1;
    
    g->names[g->numVertices] = mystrdup(key);
    
    g->vertices[g->numVertices] = newVertexList(g->numVertices);
    g->numVertices++;
    
    return g->numVertices - 1; 

}

/*
 * Returns TRUE if a key exists in names
 */ 

int graphKeyExists(Graph g, char *key) {
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        if (g->names[i] != NULL && strcmp(g->names[i], key) == 0) return TRUE;
        
        i++;
        
    }
    
    return FALSE;
    
}

/*
 * Adds a directional connection from src to dst on the graph g
 * Assumes src exists
 */

void graphConnectVertices(Graph g, int src,  int dst) {
    
    assert(g != NULL);
    
    if (src == dst) return;
        
    if (!graphIsIDValid(g, src) || !graphIsIDValid(g, dst)) return;
        
    List list = g->vertices[src];
        
    assert(list != NULL); // TODO add it?
    
    // So we dont get multiple edges between same vertex make sure it 
    // doesnt already exist in the adjacency list of src
    if (listContains(list, dst)) return;
    
    appendVertex(list, newVertex(dst));
    
    g->numEdges++;

}

/*
 * Appends the given vertex to the given list.
 */

static void appendVertex(List list, Vertex vertex) {
    
    assert(list != NULL);
    assert(vertex != NULL);
    
    // If either head or last is NULL then we have an empty list
    if (list->head == NULL || list->last == NULL) {
        
        list->head = vertex;
        list->last = vertex;
        
    } else {
        
        list->last->next = vertex;
        list->last = vertex;
        
    }
    
    list->numVertices++;
    
}

/*
 * Returns TRUE if there is an edge between the vertex with ids src and dst
 */

int graphConnectionExists(Graph g, int src, int dst) {
    
    assert(g != NULL);
    
    if (!graphIsIDValid(g, src) || !graphIsIDValid(g, dst)) return FALSE;
    
    // Get the adjacency list of src and see if dst exists in it
    
    List list = g->vertices[src];
    
    Vertex curr = list->head;
    
    while (curr != NULL) {
        
        if (curr->id == dst) return TRUE;
        
        curr = curr->next;
        
    }
    
    return FALSE;
    
}

/*
 * Returns TRUE if the given List contains the id (e.g. list->id is 
 * connected to id)
 */

static int listContains(List list, int id) {
    
    assert(list != NULL);
    
    Vertex curr = list->head;
    
    // Loop over the given list and see if key is contained
    while (curr != NULL) {
        
        if (curr->id == id) return TRUE;
        
        curr = curr->next;
        
    }
    
    return FALSE;
    
}

/*
 * Returns TRUE if the id is being used
 */

int graphIDExists(Graph g, int id) {
    
    assert(g != NULL);
    
    if (graphIsIDValid(g, id) && g->names[id] != NULL) return TRUE;
    
    return FALSE;
    
}

/*
 * Returns the number of vertices that connect to an id
 */ // TODO

static int getNumVerticesIn(Graph g, int id) {
    
    int numVertices = 0;
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        Vertex curr = g->vertices[i]->head;
        
        // Minor optimisation as we never see the same vertex
        // in a vertex adjacency twice
        int found = FALSE;
        
        while (curr != NULL && !found) {
            
            if (curr->id == id) {
                
                numVertices++;
                
                found = TRUE;
                
            }
            
            curr = curr->next;
            
        }
        
        i++;
        
    }
    
    return numVertices;
    
}

/*
 * Returns a -1 terminated array of ints (ids) that point into an id
 */

int *graphGetInlinkIDsFromID(Graph g, int id, int *num) {

    int numVertices = getNumVerticesIn(g, id);
    
    int *retArray = calloc(numVertices, sizeof(int));
    
    int currIndex = 0;
    
    int i = 0;
    
    // Loop over every vertex
    while (i < g->numVertices) {
        
        Vertex curr = g->vertices[i]->head;
        
        // Minor optimisation as we never see the same vertex
        // in a vertex adjacency twice
        int found = FALSE;
        
        // Loop over every vertex in the adjacency list
        while (curr != NULL && !found) {
            
            if (curr->id == id) {
                
                retArray[currIndex] = g->vertices[i]->id;
                
                found = TRUE;
                
                currIndex++;
                
            }
            
            curr = curr->next;
            
        }
        
        i++;
        
    }
    
    *num = numVertices;
    
    return retArray;
    
}

/*
 * Returns a -1-terminated int array of links from id
 */

int *graphGetOutlinkIDsFromID(Graph g, int id, int *num) {
    
    assert(g != NULL);
    
    if (!graphIsIDValid(g, id)) return NULL;
    
    int i = 0;
    
    int *retArray = calloc(g->vertices[id]->numVertices, sizeof(int));
    
    Vertex curr = g->vertices[id]->head;
    
    while (curr != NULL) {
        
        retArray[i] = curr->id;
        
        curr = curr->next;
        
        i++;
        
    } 
    
    *num = g->vertices[id]->numVertices;
    
    return retArray;
    
}

/*
 * Prints the graph
 */

void graphPrint(Graph g) {
    
    assert(g != NULL);
    
    int i = 0;
    
    // Loop over every vertex entry
    while (i < g->numVertices && g->vertices[i] != NULL) {
        
        printf("%s", g->names[g->vertices[i]->id]);
        
        Vertex curr = g->vertices[i]->head;
        
        // Then loop over every adjacency that it has and print them
        while (curr != NULL) {
            
            printf(" -> %s", g->names[curr->id]);
            
            curr = curr->next;
            
        }
        
        // X denotes NULL
        printf(" -> X\n");
        
        printf("|\n");
        printf("v\n");
        
        i++;
        
    }
    
    printf("X\n");
    
}

/*
 * Returns the assosiated id to a key
 */

int graphKeyToID(Graph g, char *key) {
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        if (g->names[i] != NULL && strcmp(g->names[i], key) == 0)
            return i;
        
        i++;
        
    }
    
    return NO_KEY;
    
}

/*
 * Returns the assosiated key to an id.
 */

char *graphIDToKey(Graph g, int id) {
    
    if (!graphIsIDValid(g, id)) return NULL;
    
    return g->names[id];
    
}

/*
 * Returns TRUE if the given ID is valid
 */

int graphIsIDValid(Graph g, int id) {
    
    if (id >= 0 && id < g->numVertices) return TRUE;
    
    return FALSE;
    
}

/*
 * Prints a NULL terminated char **
 */

void printTokens(char **tokens) {
    
    int i = 0;
    
    while (tokens[i] != NULL) {
        
        printf("%s\n", tokens[i]);
        
        i++;
        
    }
    
}

/*
 * As strdup does not exist in c11
 */

static char *mystrdup(char *string) {
    
    assert(string != NULL);
    
    int len = strlen(string);
    
    // +1 for nul-terminator
    char *retString = calloc(len + 1, sizeof(char));
    
    assert(retString != NULL);
    
    strcat(retString, string);
    
    return retString;
    
}

/*
 * Simple set of tests to check that the graph is working
 */

void graphTest() {
    
    printf("Testing creation of graph\n");
    
    Graph g = graphCreate(10);
    
    printf("Testing adding keys to graph\n");
    
    graphAddVertex(g, "a");
    assert(graphKeyExists(g, "a"));

    graphAddVertex(g, "b");
    assert(graphKeyExists(g, "b"));

    graphAddVertex(g, "c");
    assert(graphKeyExists(g, "c"));

    graphAddVertex(g, "d");
    assert(graphKeyExists(g, "d"));
    
    printf("Testing adding two vertices with same key\n");
    
    graphAddVertex(g, "e");
    assert(graphKeyExists(g, "e"));
    graphAddVertex(g, "e");
    
    printf("Testing connecting vertices of graph\n");
    
    graphConnectVertices(g, graphKeyToID(g, "a"), graphKeyToID(g, "b"));
    assert(graphConnectionExists(g, graphKeyToID(g, "a"), graphKeyToID(g, "b")));
    
    graphConnectVertices(g, graphKeyToID(g, "a"), graphKeyToID(g, "c"));
    assert(graphConnectionExists(g, graphKeyToID(g, "a"), graphKeyToID(g, "c")));
    
    graphConnectVertices(g, graphKeyToID(g, "a"), graphKeyToID(g, "d"));
    assert(graphConnectionExists(g, graphKeyToID(g, "a"), graphKeyToID(g, "d")));
    
    graphConnectVertices(g, graphKeyToID(g, "d"), graphKeyToID(g, "b"));
    assert(graphConnectionExists(g, graphKeyToID(g, "d"), graphKeyToID(g, "b")));

    graphConnectVertices(g, graphKeyToID(g, "d"), graphKeyToID(g, "a"));
    assert(graphConnectionExists(g, graphKeyToID(g, "d"), graphKeyToID(g, "b")));
    
    graphConnectVertices(g, graphKeyToID(g, "e"), graphKeyToID(g, "c"));
    assert(graphConnectionExists(g, graphKeyToID(g, "e"), graphKeyToID(g, "c")));
    
    printf("Testing adding two edges between same vertices\n");
    
    graphConnectVertices(g, graphKeyToID(g, "d"), graphKeyToID(g, "c"));
    assert(graphConnectionExists(g, graphKeyToID(g, "d"), graphKeyToID(g, "c")));
    graphConnectVertices(g, graphKeyToID(g, "d"), graphKeyToID(g, "c"));

    printf("Testing self connections\n");
    
    graphConnectVertices(g, graphKeyToID(g, "d"), graphKeyToID(g, "d"));
    assert(!graphConnectionExists(g, graphKeyToID(g, "d"), graphKeyToID(g, "d")));
    
    assert(getNumVerticesIn(g, graphKeyToID(g, "e")) == 0);
    assert(getNumVerticesIn(g, graphKeyToID(g, "d")) == 1);
    assert(getNumVerticesIn(g, graphKeyToID(g, "a")) == 1);
    assert(getNumVerticesIn(g, graphKeyToID(g, "z")) == 0);
    
    graphPrint(g);
    
    
    graphFree(g);
    
}
