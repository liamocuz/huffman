#include "header.h"
#include "encode.h"

int compress(char* input, char* output) {
    long int uniqueChars = 0;
    // long int totalCharsCompressed = 0; // idk if I even need this
    long int totalCharsUncompressed = 0;
    int asciiCount[ASCII_SIZE];
    char* table[ASCII_SIZE];
    Node* root = NULL;

    // FILE* inptr = NULL;
    // FILE* outptr = NULL;

    // Make sure all counts are set to 0
    memset(asciiCount, 0, sizeof(asciiCount));

    if ((countChars(input, asciiCount, &uniqueChars, &totalCharsUncompressed)) != ENCODE_SUCCESS) {
        return ENCODE_FAILURE;
    }

    // DEBUG
    printf("unique: %li, totalUncompressed: %li\n", uniqueChars, totalCharsUncompressed);

    if ((root = createTree(uniqueChars, asciiCount)) == NULL) {
        printf("Error: Could not build tree.\n");
        return ENCODE_FAILURE;
    }

    initTable(table);

    if (buildTableFromTree(root, table) != ENCODE_SUCCESS) {
        printf("Error: Could not create table.\n");
        return ENCODE_FAILURE;
    }

    // Time to now compress the file
    if (compressFile(input, output, table) != ENCODE_SUCCESS) {
        printf("Error: Could not compress file.\n");
        return ENCODE_FAILURE;
    }

    freeTree(root);
    freeTable(table);

    return ENCODE_SUCCESS;
}

// Counts the occurence of each char from an input file
int countChars(char* filename, int* asciiCount, long int* uniqueChars, long int* totalCharsUncompressed) {
    FILE* fptr = NULL;
    char ch;

    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("Error: Unable to open filename %s in countChars.\n", filename);
        return ENCODE_FAILURE;
    }

    while ((ch = fgetc(fptr)) != EOF) {
        if (asciiCount[(int)ch] == 0)
            (*uniqueChars)++;
        asciiCount[(int)ch]++;
        (*totalCharsUncompressed)++;
    }

    // DEBUG
    // for (int i = 0; i < ASCII_SIZE; i++) {
    //     printf("char: %c, weight: %d\n", i, asciiCount[i]);
    // }
    // printf("unique: %ld\n", uniqueChars);

    fclose(fptr);

    return ENCODE_SUCCESS;
}

int compressFile(char* infile, char* outfile, char** table) {
    FILE* inptr = NULL;
    FILE* outptr = NULL;
    char* code = NULL;
    char ch = 0;
    int i = 0;
    unsigned char byte = 0xff;
    int written = 0;
    char one = '1';
    char zero = '0';

    if ((inptr = fopen(infile, "r")) == NULL) {
        printf("Error: Unable to open filename %s in compressFile.\n", infile);
        return ENCODE_FAILURE;
    }

    if ((outptr = fopen(outfile, "w")) == NULL) {
        printf("Error: Unable to open filename %s in compressFile.\n", outfile);
        return ENCODE_FAILURE;
    }

    while ((ch = fgetc(inptr)) != EOF) {
        code = table[(int)ch];
        i = 0;
        ch = code[i];
        while (ch != '\0') {
            if (written == 8) {
                fputc(byte, outptr);
                written = 0;
                byte = 0xff;
            }
            if (ch == zero) {
               byte >>= 1;
               written++; 
            }
            else if (ch == one) {
                byte |= (0x80 >> 1);
                written++;
            }
            i++;
            ch = code[i];
        }
    }

    return ENCODE_SUCCESS;
}