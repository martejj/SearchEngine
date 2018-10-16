#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

int main(int argc, char *argv[]) {
    
    double d = 0;
    double diffPR = 0;
    int maxIters = 0;
    
    if (argc != 4) {
        
        printf("Usage: %s dampingFactor maxdiff maxiters\n", argv[0]);
        exit(1);
        
    } else {
        
        sscanf(argv[1], "%lf", &d);
        sscanf(argv[2], "%lf", &diffPR);
        maxIters = atoi(argv[3]);
        
    }
    
    printf("got: %lf, %.7lf, %d\n", d, diffPR, maxIters);
    
}