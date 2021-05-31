#include "header.h"
#include "encode.h"

// Counts the occurence of each char from an input file
long int countChars(char* filename, int* asciiCount) {
    FILE* fptr = NULL;
    char ch;
    long int uniqueChars = 0;

    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("Error: Unable to open filename %s in countChars.\n", filename);
        return ENCODE_FAILURE;
    }

    while ((ch = fgetc(fptr)) != EOF) {
        if (asciiCount[(int)ch] == 0)
            uniqueChars++;
        asciiCount[(int)ch]++;
    }

    // DEBUG
    // for (int i = 0; i < ASCII_SIZE; i++) {
    //     printf("char: %c, weight: %d\n", i, asciiCount[i]);
    // }
    // printf("unique: %ld\n", uniqueChars);

    return uniqueChars;
}

int compress(char* input, char* output) {
    
    long int uniqueChars = 0;
    int asciiCount[ASCII_SIZE];
    Node* root = NULL;

    // Make sure all counts are set to 0
    memset(asciiCount, 0, sizeof(asciiCount));

    if ((uniqueChars = countChars(input, asciiCount)) == ENCODE_FAILURE) {
        return ENCODE_FAILURE;
    }

    root = createTree(uniqueChars, asciiCount);

    return ENCODE_SUCCESS;
}

// Returns tree root
Node* createTree(long int uniqueChars, int* asciiCount) {
    int i = 0;
    int j = 0;

    Node* arr = malloc(uniqueChars * sizeof(Node));
    if (arr == NULL) {
        printf("Error: malloc returns NULL in createTree.\n");
        return NULL;
    }

    for (i = 0; i < ASCII_SIZE; i++) {
        if (asciiCount[i] != 0) {
            arr[j].c = i;
            arr[j].weight = asciiCount[i];
            arr[j].left = NULL;
            arr[j].right = NULL;
            j++;
        }
    }

    sortTreeArray(arr, uniqueChars);

    

    //printArr(arr, uniqueChars);

    free(arr);


    return NULL;
}