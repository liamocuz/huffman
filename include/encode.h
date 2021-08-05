#include "tree.h"

// Function definitions
int countChars(char* input, int* asciiCount, long int* uniqueChars, long int* totalCharsUncompressed);
int compress(char* input, char* output);
int compressFile(char* infile, char* outfile, char** table);