#include "tree.h"

// Encoding Function Definitions
int countChars(char* input, int* asciiCount, long int* uniqueChars, long int* totalCharsUncompressed);
int compress(char* input, char* output);
int compressFile(char* infile, char* outfile, char** table);

// Decoding Function Definitions
int decode(char* input, char* output);
int decompress(char* output, Node* root, unsigned char* encoding, long int numCharsUncomp);
void buildTreeFromEncoding(Node* root, unsigned char* encoding, int* i, long int numCharsTopology, int* numCharsPut);
void parseByteEncoding(unsigned char* byteEncoding, unsigned char* strEncoding, long int numCharsTopology);
void initNode(Node* node);