// Constants
#define ENCODE_SUCCESS 1
#define ENCODE_FAILURE -1

#define ASCII_SIZE 128

// Function definitions
long int countChars(char* input, int* asciiCount);
int compress(char* input, char* output);
Node* createTree(long int uniqueChars, int* asciiCount);