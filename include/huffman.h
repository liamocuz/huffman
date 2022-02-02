// Gonna be main header
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Constants
#define ENCODE_SUCCESS 1
#define ENCODE_FAILURE -1
#define ASCII_SIZE 128

// Struct Definitions

/*
    The Node struct is used to construct huffman code trees
*/
typedef struct Node {
    struct Node* left;
    struct Node* right;
    int weight;
    char c;
} Node;

/*
    The List struct is used to build and sort Nodes during the construction of the huffman encoding tree 
*/
typedef struct List {
    struct Node* node;
    struct List* next;
} List;

/*
    The Header struct just stores 3 long ints that are used to decode a huffman file
*/
typedef struct Header {
    long int numCharsComp;      // Number of chars in compressed file 
    long int numCharsTopology;  // Number of chars storing the topology
    long int numCharsUncomp;    // Number of chars in the uncompressed file
} Header;

// Encoding Function Definitions
int compress(char* input, char* output);
int countChars(char* input, int* asciiCount, long int* uniqueChars, long int* totalCharsUncompressed);
Node* createTree(long int uniqueChars, int* asciiCount);
void sortTreeArray(List* head);
List* buildTreeFromList(List* head);
Node* joinNodes(Node* a, Node* b);
void swap(Node* a, Node* b);
void initTable(char** table);
void preOrderTraversal(Node* node, char* gather, int* gatherSize, char** table, char* topology, int* topologySize, int* asciiCount, long int* numCharsComp);
int writeHeader(Header* header, char* outputFile);
int writeTopology(char* topology, char* output);
int compressFile(char* infile, char* outfile, char** table);

// Decoding Function Definitions
int decompress(char* input, char* output);
void parseByteTopology(unsigned char* byteTopology, char* stringTopology, long int numCharsTopology);
int decompressFile(char* input, int offset, char* output, Node* root, long int numCharsUncomp);
void buildTreeFromTopology(Node* root, char* topology, int* i, long int numCharsTopology, int* numCharsPut);
void initNode(Node* node);

// Helper Functions
void freeArr(List* head);
void freeTree(Node* root);
void freeTable(char** table);
void printArr(List* head);
void printTable(char** table);
void preOrderPrint(Node* node);
