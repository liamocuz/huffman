#include "header.h"
#include "encode.h"

void initTable(char** table) {
   int i = 0;
   for (i = 0; i < ASCII_SIZE; i++) {
       table[i] = NULL;
   }
}

int buildTableFromTree(Node* root, char** table, Header* header, char* output) {
    char encoding[ENCODING_SIZE];
    char gather[GATHER_SIZE];
    FILE* outptr = NULL;

    memset(encoding, 0, ENCODING_SIZE);
    memset(gather, 0, GATHER_SIZE);

    preOrderTraversal(root, encoding, gather, table, &header->numCharsEncoding);

    if ((outptr = fopen(output, "w")) == NULL) {
        printf("Error: Unable to open file %s.\n", output);
        return ENCODE_FAILURE;
    }
    fseek(outptr, 0, SEEK_END);
    fwrite(header, sizeof(Header), 1, outptr);
    fclose(outptr);

    // DEBUG
    printf("\nENCODING\n");
    printf("%s\n", encoding);
    
    if ((writeEncoding(encoding, output)) == ENCODE_FAILURE) {
        printf("Error: Unable to write encoding.\n");
        return ENCODE_FAILURE;
    }

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