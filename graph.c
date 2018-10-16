#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "graph.h"

#define TRUE    1
#define FALSE   0

typedef struct _list *List;
typedef struct _vertex *Vertex;

// A  list contains all out links of the key
struct _list {
    
    char *key;
    int numVertices;
    Vertex head;
    Vertex last;
    
};

struct _vertex {
    
    char *key;
    Vertex next;
    
};

struct _graph {
    
    int maxVertices;
    int numVertices;
    int numEdges;
    List *vertices;
    
};

static void listFree(List list);

static Vertex newVertex(char *key);
static List newVertexList(char *key);

static void appendVertex(List list, Vertex vertex);
static List getAdjacencyListFromKey(Graph g, char *key);

static int listContains(List list, char *key);

// As strdup is not available in -std=c11
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
    
    assert(g->vertices != NULL);
    
    return g;
    
}

// Takes in a stack key and allocates and returns a vertex w/ heap key
static Vertex newVertex(char *key) {
    
    Vertex new = calloc(1, sizeof(*new));
    
    assert(new != NULL);
    
    new->key = mystrdup(key);
    
    return new;
    
}

static List newVertexList(char *key) {
    
    List new = calloc(1, sizeof(*new));
    
    assert(new != NULL);
    
    new->key = mystrdup(key);
    
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
    free(g);
        
}

static void listFree(List list) {

    assert(list != NULL);
    assert(list->key != NULL);
    
    Vertex curr = list->head;
    Vertex prev = NULL;
    
    while (curr != NULL) {
        
        prev = curr;
        curr = curr->next;
        
        free(prev->key);
        free(prev);
        
    }
    
    free(list->key);
    free(list);
    
}

/*
 * Adds a place in the vertex adjacency list for this key
 */

void graphAddVertex(Graph g, char *key) {
    
    assert(g != NULL);
    assert(key != NULL);
    
    // So that we dont have two entries for the same key
    if (graphKeyExists(g, key)) return;
    
    g->vertices[g->numVertices] = newVertexList(key);
    g->numVertices++;

}

/*
 * Adds a directional connection from src to dst on the graph g
 * Assumes src exists
 */

void graphConnectVertices(Graph g, char *src,  char *dst) {
    
    assert(src != NULL);
    assert(dst != NULL);
    assert(g != NULL);
        
    List list = getAdjacencyListFromKey(g, src);
        
    assert(list != NULL); // TODO
    
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
 * Returns TRUE if there is an edge between the vertex with keys src and dst
 */

int graphConnectionExists(Graph g, char *src,  char *dst) {
    
    assert(g != NULL);
    
    // Get the adjacency list of src and see if dst exists in it
    
    List list = getAdjacencyListFromKey(g, src);
    
    Vertex curr = list->head;
    
    while (curr != NULL) {
        
        if (strcmp(curr->key, dst) == 0) return TRUE;
        
        curr = curr->next;
        
    }
    
    return FALSE;
    
}

/*
 * Returns TRUE if the given List contains the key (e.g. list->key is 
 * connected to key)
 */

static int listContains(List list, char *key) {
    
    assert(list != NULL);
    
    Vertex curr = list->head;
    
    // Loop over the given list and see if key is contained
    while (curr != NULL) {
        
        if (strcmp(curr->key, key) == 0) return TRUE;
        
        curr = curr->next;
        
    }
    
    return FALSE;
    
}

/*
 * Returns TRUE if the graph contains the key key
 */

int graphKeyExists(Graph g, char *key) {
    
    assert(g != NULL);
    assert(key != NULL);
    
    int i = 0;
    
    // Loop over vertices list and see if it is contained
    while (i < g->numVertices) {
        
        // If it isnt null and the keys are the same then it exists
        if (g->vertices[i] != NULL && strcmp(key, g->vertices[i]->key) == 0) return TRUE;
        
        i++;
        
    }
    
    return FALSE;
    
}

/*
 * Returns the number of vertices that connect to a key
 */

static int getNumVerticesIn(Graph g, char *key) {
    
    int numVertices = 0;
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        Vertex curr = g->vertices[i]->head;
        
        // Minor optimisation as we never see the same vertex
        // in a vertex adjacency twice
        int found = FALSE;
        
        while (curr != NULL && !found) {
            
            if (strcmp(curr->key, key) == 0) {
                
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
 * Returns a NULL terminated array of pointers to keys that point to the
 * given key
 */

char **graphGetVerticesIn(Graph g, char *key) {

    int numVertices = getNumVerticesIn(g, key);
    
    // + 1 so we can add the NULL to the end
    char **tokens = calloc(numVertices + 1, sizeof(char *));
    
    int currToken = 0;
    
    int i = 0;
    
    // Loop over every vertex
    while (i < g->numVertices) {
        
        Vertex curr = g->vertices[i]->head;
        
        // Minor optimisation as we never see the same vertex
        // in a vertex adjacency twice
        int found = FALSE;
        
        // Loop over every vertex in the adjacency list
        while (curr != NULL && !found) {
            
            if (strcmp(curr->key, key) == 0) {
                
                // TODO strdup or use same pointer?
                tokens[currToken] = mystrdup(g->vertices[i]->key);
                
                found = TRUE;
                
                currToken++;
                
                
            }
            
            curr = curr->next;
            
        }
        
        i++;
        
    }
    
    tokens[numVertices + 1] = NULL;
    
    return tokens;
    
}

/*
 * Returns the adjacency list of a given Key
 */

static List getAdjacencyListFromKey(Graph g, char *key) {
    
    assert(g != NULL);
    assert(key != NULL);
    
    int i = 0;
    
    while (i < g->numVertices) {
        
        // If it isnt null and the keys are the same then it is the key
        if (g->vertices[i] != NULL && strcmp(key, g->vertices[i]->key) == 0) 
            return g->vertices[i];
        
        i++;
        
    }
    
    return NULL;
    
}

/*
 * Prints the graph
 */

void graphPrint(Graph g) {
    
    assert(g != NULL);
    
    int i = 0;
    
    // Loop over every vertex entry
    while (i < g->numVertices && g->vertices[i] != NULL) {
        
        printf("%s", g->vertices[i]->key);
        
        Vertex curr = g->vertices[i]->head;
        
        // Then loop over every adjacency that it has and print them
        while (curr != NULL) {
            
            printf(" -> %s", curr->key);
            
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
    
    graphConnectVertices(g, "a", "b");
    assert(graphConnectionExists(g, "a", "b"));
    
    graphConnectVertices(g, "a", "c");
    assert(graphConnectionExists(g, "a", "c"));
    
    graphConnectVertices(g, "a", "d");
    assert(graphConnectionExists(g, "a", "d"));
    
    graphConnectVertices(g, "d", "b");
    assert(graphConnectionExists(g, "d", "b"));

    graphConnectVertices(g, "d", "a");
    assert(graphConnectionExists(g, "d", "b"));
    
    graphConnectVertices(g, "e", "c");
    assert(graphConnectionExists(g, "e", "c"));
    
    printf("Testing adding two edges between same vertices\n");
    
    graphConnectVertices(g, "d", "c");
    assert(graphConnectionExists(g, "d", "c"));
    graphConnectVertices(g, "d", "c");

    printf("Testing self connections\n");
    
    graphConnectVertices(g, "d", "d");
    assert(graphConnectionExists(g, "d", "d"));
    graphConnectVertices(g, "d", "d");   
    
    assert(getNumVerticesIn(g, "e") == 0);
    assert(getNumVerticesIn(g, "d") == 2);
    assert(getNumVerticesIn(g, "a") == 1);
    assert(getNumVerticesIn(g, "z") == 0);
    
    printTokens(graphGetVerticesIn(g, "e"));
    printTokens(graphGetVerticesIn(g, "d"));
    printTokens(graphGetVerticesIn(g, "a"));
    printTokens(graphGetVerticesIn(g, "z"));
    
    graphPrint(g);
    
    
    graphFree(g);
    
}
