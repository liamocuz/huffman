#include "tree.h"

// Encoding Function Definitions
int countChars(char* input, int* asciiCount, long int* uniqueChars, long int* totalCharsUncompressed);
int compress(char* input, char* output);
int compressFile(char* infile, char* outfile, char** table);

// Decoding Function Definitions
int decode(char* input, char* output);
Node* buildTreeFromEncoding(char* encoding, long int numCharsTopology);