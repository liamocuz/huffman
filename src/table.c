#include "header.h"
#include "encode.h"

// Inits the table, which is an array of char arrays
void initTable(char** table) {
   int i = 0;
   for (i = 0; i < ASCII_SIZE; i++) {
       table[i] = NULL;
   }
}

// This function is more of a wrapper for the preOrderTraversal function
int buildTableFromTree(Node* root, char** table, char* topology, Header* header, int* asciiCount) {
    /*
    Can do this iteratively
    Keep searching the tree until all leaf nodes have been hit, which the count is stored in header
    Must track the current size and previous nodes via a heap
    Will use the linked list struct to create a heap
    Do a PreOrderTraversal of the tree, using the heap as a way to keep track of previously visited nodes
    When going left, concatenate a '0', right concatenate a '1'
    When a leaf node is visited, place that code in the table at the location of the (int)char in leaf node
    */

    /* 
    But can also do this recursively, which honestly is more natural for a tree traversal
    PreOrderTraversal goes root, left, right
    If at leaf node, copy the string encoding to the corresponding place in the table of the int value of the char in the leaf node
    If encountered a leaf node, concatenate a '1' and then the char
    If encountered a non-leaf node, concatenate a '0'
    */
    
    char gather[GATHER_SIZE];   // Memory space for gathering the huffman code string
    int gatherSize = 0;         // Start the gather size of the string at 0
    int topologySize = 0;       // Start the topology size of the string at 0

    
    memset(gather, 0, GATHER_SIZE * sizeof(char));

    preOrderTraversal(root, gather, &gatherSize, table, topology, &topologySize, asciiCount, &header->numCharsComp);
  
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