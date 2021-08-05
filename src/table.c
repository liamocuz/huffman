#include "header.h"
#include "encode.h"

void initTable(char** table) {
   int i = 0;
   for (i = 0; i < ASCII_SIZE; i++) {
       table[i] = NULL;
   }
}

int buildTableFromTree(Node* root, char** table, char* output) {
    char encoding[ENCODING_SIZE];
    char gather[GATHER_SIZE];
    FILE* outptr = NULL;

    memset(encoding, 0, ENCODING_SIZE);
    memset(gather, 0, GATHER_SIZE);

    preOrderTraversal(root, encoding, gather, table);

    if ((outptr = fopen(output, "a")) == NULL) {
        printf("Error: Unable to open file %s.\n", output);
        return ENCODE_FAILURE;
    }

    fputs(encoding, outptr);

    fclose(outptr);

    // DEBUG
    // printf("\nENCODING\n\n");
    // printf("%s\n", encoding);


    // DEBUG
    // printf("\nTABLE\n\n");
    // printTable(table);

    return ENCODE_SUCCESS;
}

void printTable(char** table) {
    int i = 0;
    for (i = 0; i < ASCII_SIZE; i++) {
        if (table[i] != NULL) {
            printf("%c: %s\n", (char)i, table[i]);
        }
    }
}

void freeTable(char** table) {
    int i = 0;

    for (i = 0; i < ASCII_SIZE; i++) {
        if (table[i]) {
            free(table[i]);
        }
    }
}