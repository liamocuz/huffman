#include "tree.h"

// Constants
#define ENCODE_SUCCESS 1
#define ENCODE_FAILURE -1

// Function definitions
int countChars(char* input, int* asciiCount, long int* uniqueChars, long int* totalCharsUncompressed);
int compress(char* input, char* output);
int compressFile(char* infile, char* outfile, char** table);