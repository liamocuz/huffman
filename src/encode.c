#include "header.h"
#include "encode.h"

int compress(char* input, char* output) {
    int asciiCount[ASCII_SIZE];
    char* table[ASCII_SIZE];
    Node* root = NULL;
    Header header;
    header.numCharsEncoding = 0;
    header.numCharsTopology = 0;
    header.numCharsUncomp = 0;

    // FILE* inptr = NULL;
    FILE* outptr = NULL;

    // Make sure all counts are set to 0
    memset(asciiCount, 0, sizeof(asciiCount));

    if ((countChars(input, asciiCount, &header.numCharsTopology, &header.numCharsUncomp)) != ENCODE_SUCCESS) {
        return ENCODE_FAILURE;
    }

    // DEBUG
    // printf("numCharsEncoding: %li, numCharsUncomp: %li\n", header.numCharsTopology, header.numCharsUncomp);

    if ((root = createTree(header.numCharsTopology, asciiCount)) == NULL) {
        printf("Error: Could not build tree.\n");
        return ENCODE_FAILURE;
    }

    initTable(table);

    if (buildTableFromTree(root, table, &header.numCharsEncoding, output) != ENCODE_SUCCESS) {
        printf("Error: Could not create table.\n");
        return ENCODE_FAILURE;
    }

    if ((outptr = fopen(output, "w")) == NULL) {
        printf("Error: Unable to open file %s.\n", output);
        return ENCODE_FAILURE;
    }
    fwrite(&header, 1, sizeof(header), outptr);
    fclose(outptr);

    // DEBUG
    // printf("numCharsEncoding: %li\n", header.numCharsEncoding);

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
        printf("Error: Unable to open file %s.\n", filename);
        return ENCODE_FAILURE;
    }

    while ((ch = fgetc(fptr)) != EOF) {
        if (asciiCount[(int)ch] == 0) {
            (*uniqueChars)++;

            // DEBUG
            // printf("count ch: %c, val: %d\n", ch, (int)ch);
        }
        asciiCount[(int)ch]++;
        (*totalCharsUncompressed)++;
    }

    // DEBUG
    // int entry = 1;
    // printf("\nWEIGHTS\n\n");
    // for (int i = 0; i < ASCII_SIZE; i++) {
    //     if (asciiCount[i]) {
    //         printf("num: %d, int: %d: char: %c, weight: %d\n", entry, i, (char)i, asciiCount[i]);
    //         entry++;
    //     }
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
    unsigned char byte = 0x00;
    int shifts = 0;
    char one = '1';
    char zero = '0';

    if ((inptr = fopen(infile, "r")) == NULL) {
        printf("Error: Unable to open file %s.\n", infile);
        return ENCODE_FAILURE;
    }

    if ((outptr = fopen(outfile, "a")) == NULL) {
        printf("Error: Unable to open file %s.\n", outfile);
        return ENCODE_FAILURE;
    }

    while ((ch = fgetc(inptr)) != EOF) {
        code = table[(int)ch];
        i = 0;
        ch = code[i];

        // DEBUG
        // printf("ch: %c\n", ch);

        // DEBUG
        while (ch != '\0') {
            // printf("num: %d\n", shifts + 1);
            // printf("ch: %c\n", ch);
            // printf("byte before: %x\n", byte);
            if (ch == one) {
                byte >>= 1;
                byte |= 0x80;
                shifts++;
            }
            else if (ch == zero) {
                byte >>= 1;
                shifts++;
            }
            else {
                printf("Error: Invalid character %c %d retrieved from code string!\n", ch, (int)ch);
                fclose(inptr);
                fclose(outptr);
                return ENCODE_FAILURE;
            }

            if (shifts == 8) {
                // printf("\nshifts Byte\n");
                // printf("Byte: %x\n", byte);
                fputc(byte, outptr);
                shifts = 0;
                byte = 0x00;
            }
            // printf("byte after: %x\n", byte);
            i++;
            ch = code[i];
        }        
    }

    while (shifts != 8) {
        byte >>= 1;
        shifts++;
    }
    fputc(byte, outptr);

    fclose(inptr);
    fclose(outptr);

    return ENCODE_SUCCESS;
}