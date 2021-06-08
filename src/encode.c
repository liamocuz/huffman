#include "header.h"
#include "encode.h"

int compress(char* input, char* output) {
    long int uniqueChars = 0;
    // long int totalCharsCompressed = 0; // idk if I even need this
    long int totalCharsUncompressed = 0;
    int asciiCount[ASCII_SIZE];
    char* table[ASCII_SIZE];
    Node* root = NULL;

    // FILE* inptr = NULL;
    // FILE* outptr = NULL;

    // Make sure all counts are set to 0
    memset(asciiCount, 0, sizeof(asciiCount));

    if ((countChars(input, asciiCount, &uniqueChars, &totalCharsUncompressed)) != ENCODE_SUCCESS) {
        return ENCODE_FAILURE;
    }

    // DEBUG
    printf("unique: %li, totalUncompressed: %li\n", uniqueChars, totalCharsUncompressed);

    if ((root = createTree(uniqueChars, asciiCount)) == NULL) {
        printf("Error: Could not build tree.\n");
        return ENCODE_FAILURE;
    }

    if (buildTableFromTree(root, table) != ENCODE_SUCCESS) {
        printf("Error: Could not create table.\n");
        return ENCODE_FAILURE;
    }

    

    freeTree(root);

    return ENCODE_SUCCESS;
}

// Counts the occurence of each char from an input file
int countChars(char* filename, int* asciiCount, long int* uniqueChars, long int* totalCharsUncompressed) {
    FILE* fptr = NULL;
    char ch;

    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("Error: Unable to open filename %s in countChars.\n", filename);
        return ENCODE_FAILURE;
    }

    while ((ch = fgetc(fptr)) != EOF) {
        if (asciiCount[(int)ch] == 0)
            (*uniqueChars)++;
        asciiCount[(int)ch]++;
        (*totalCharsUncompressed)++;
    }

    // DEBUG
    // for (int i = 0; i < ASCII_SIZE; i++) {
    //     printf("char: %c, weight: %d\n", i, asciiCount[i]);
    // }
    // printf("unique: %ld\n", uniqueChars);

    fclose(fptr);

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

    // DEBUG
    // printArr(head);

    head = buildTreeFromList(head);
    root = head->node;
    free(head);

    // DEBUG
    // preOrderPrint(root);
    // freeTree(root);

    return root;
}