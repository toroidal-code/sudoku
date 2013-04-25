#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#define DIM 9
#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

int** createArray(int m, int n){
    int* values = calloc(m*n, sizeof(int));
    int** rows = malloc(n*sizeof(int*));
    for (int i=0; i<n; ++i)
    {
        rows[i] = values + i*m;
    }
    return rows;
}

void destroyArray(int** arr){
    free(*arr);
    free(arr);
}

void printArray(int** array){
    /* Printing array */
    for (int row = 0; row < DIM; row++) {
        for (int col = 0; col < DIM; col++) {
            printf(" %d", array[row][col]);
        }
        printf("\n");
    }

}

int** readFile(char* file){
    FILE *mapfile = fopen(file, "r");
    int** array = createArray(DIM,DIM);
    char charbuffer[3];
    /* Reading file into array */
    for (int row = 0; row < DIM; row++) {
        for (int col = 0; col < DIM; col++) {
            if (fgets(charbuffer, 3, mapfile))
                array[row][col] = atoi(strdup(charbuffer));
        }
    }
    fclose(mapfile);    
    return array;   
}

int* removeVals(int* list,int val){
    int* temp[LENGTH(list)] = {};
    for (int i = 0; i < LENGTH(list); i++){
        if (list[i] != val){
            temp[i] = list[i];
        }
    }
}

bool isGoal(int** config){

}

int*** getSuccessors(int** config){
    
}

int** solve(int** config,bool debug){
    if (isGoal(config) == true ){
        return config;
    } else {
        if (debug) {printf("Current:\n "); printArray(config);}
        int*** successorArray = getSuccessors(config);
        for (int i = 0; i < LENGTH(successorArray); i ++){
            int** successor = successorArray[i]; 
        }
    }
}

int main(int argc, char const *argv[]) {
    //load the board into a 2D array
    int** initConfig = readFile("su0.txt");

    //Print the initial state
    printf("Initial Config:\n");
    printArray(initConfig);

    //Destroy array
    destroyArray(initConfig);
    return 0;
}
