// Gonna be main header
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Header struct

typedef struct Header {
    long int numCharsComp;
    long int numUniqueChars;
    long int numCharsUncomp;
} Header;

// Constants
#define ASCII_SIZE 128