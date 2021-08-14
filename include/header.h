// Gonna be main header
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Constants
#define ENCODE_SUCCESS 1
#define ENCODE_FAILURE -1
#define GATHER_SIZE 128
#define ENCODING_SIZE 1024

// Header struct

typedef struct Header {
    long int numCharsEncoding;  // Length of the encoding string
    long int numCharsTopology;  // Number of chars storing the topology
    long int numCharsUncomp;    // Number of chars in the uncompressed file
} Header;

// Constants
#define ASCII_SIZE 128