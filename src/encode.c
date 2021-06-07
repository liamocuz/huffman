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

    fclose(fptr);

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
    List* head = NULL;
    List* ltemp = NULL;
    Node* ntemp = NULL;
    Node* root = NULL;

    for (i = 0; i < ASCII_SIZE; i++) {
        if (asciiCount[i] != 0) {
            if (head == NULL) {
                head = (List*)malloc(sizeof(List));

                ntemp = (Node*)malloc(sizeof(Node));
                ntemp->c =(char)i;
                ntemp-> weight = asciiCount[i];
                ntemp->left = NULL;
                ntemp->right = NULL;

                ltemp = head;
                ltemp->node = ntemp;
                ltemp->next = NULL;
            }
            else {
                ltemp->next = (List*)malloc(sizeof(List));
                ltemp = ltemp->next;

                ntemp = (Node*)malloc(sizeof(Node));
                ntemp->c = (char)i;
                ntemp-> weight = asciiCount[i];
                ntemp->left = NULL;
                ntemp->right = NULL;

                ltemp->node = ntemp;
                ltemp->next = NULL;
            }
        }
    }

    sortTreeArray(head);

    printArr(head);

    head = buildTreeFromList(head);
    root = head->node;
    free(head);

    preOrderPrint(root);

    freeTree(root);


    return NULL;
}