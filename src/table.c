#include "header.h"
#include "encode.h"

void initTable(char** table) {
   int i = 0;
   for (i = 0; i < ASCII_SIZE; i++) {
       table[i] = NULL;
   }
}

int buildTableFromTree(Node* root, char** table) {
    char encoding[1024];
    char gather[128];
    int len = 0;
    int flip = 1;
    char runner = '\0';
    const char one = '1';
    const char zero = '0';
    int i = 0;

    memset(encoding, 0, 1024);
    memset(gather, 0, 128);

    preOrderTraversal(root, encoding);

    // DEBUG
    printf("%s\n", encoding);

    runner = encoding[i];
    while (runner != '\0') {
        runner = encoding[i];
        
        if (runner == zero) {
            strncat(gather, &zero, 1);
            len++;
        }
        else if (runner == one) {
            i++;
            runner = encoding[i];
            table[(int)runner] = (char*)malloc(sizeof(char) * (len + 1));
            table[(int)runner][len] = '\0'; 
            memcpy(table[(int)runner], gather, len);

           if (flip) {
               gather[len - 1] = one;
               flip = 0;
           }
           else {
               gather[len - 2] = one;
               gather[len - 1] = zero;
               flip = 1;
               len--;
           } 

            // DEBUG
            // printf("char: %c, table entry: %s\n", runner, table[(int)runner]);
        }
        i++;
    }

    printTable(table);

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
        if (table[i] != NULL) {
            free(table[i]);
        }
    }
}