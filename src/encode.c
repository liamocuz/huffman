#include "header.h"
#include "encode.h"

int compress(char* input, char* output) {
    int asciiCount[ASCII_SIZE];
    char* table[ASCII_SIZE];
    Node* root = NULL;
    Header header;
    header.numCharsComp = 0;
    header.numCharsTopology = 0;
    header.numCharsUncomp = 0;

    // Make sure all counts are set to 0
    memset(asciiCount, 0, sizeof(asciiCount));

    if ((countChars(input, asciiCount, &header.numCharsTopology, &header.numCharsUncomp)) != ENCODE_SUCCESS) {
        return ENCODE_FAILURE;
    }

    if ((root = createTree(header.numCharsTopology, asciiCount)) == NULL) {
        printf("Error: Could not build tree.\n");
        return ENCODE_FAILURE;
    }

    initTable(table);

    if (buildTableFromTree(root, table, &header, output) != ENCODE_SUCCESS) {
        printf("Error: Could not create table.\n");
        return ENCODE_FAILURE;
    }

    preOrderPrint(root);

    // DEBUG
    printf("numCharsComp: %li, numCharsTopology: %li, numCharsUncomp: %li\n",
        header.numCharsComp, header.numCharsTopology, header.numCharsUncomp);
    printTable(table);

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

        while (ch != '\0') {
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
                fputc(byte, outptr);
                shifts = 0;
                byte = 0x00;
            }
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

int writeEncoding (char* encoding, char* output) {
    char ch = 0;
    int shifts = 0;
    int i = 0;
    int j = 0;
    unsigned char byte = 0x00;
    const unsigned char masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    const char one = '1';
    const char zero = '0';
    FILE* outptr = NULL;

    if ((outptr = fopen(output, "a")) == NULL) {
        printf("Error: Unable to open file %s.\n", output);
        return ENCODE_FAILURE;
    }

    ch = encoding[i];
    while (ch != '\0') {
        if (ch == one) {
            byte >>= 1;
            byte |= 0x80;
            shifts++; 
        }
        else if (ch == zero) {
            byte >>= 1;
            shifts++;
        }
        else if (ch >= 1 && ch <= 127) {
            for (j = 0; j < 8; j++) {
                byte >>= 1;
                byte |= (masks[j] & ch) << (7 - j);
                shifts++;

                if (shifts == 8) {
                    putc(byte, outptr);
                    shifts = 0;
                    byte = 0x00;
                }
            }
        }
        else {
            printf("Error: Invalid character %c %d retrieved from encoding string!\n", ch, (int)ch);
            fclose(outptr);
            return ENCODE_FAILURE;
        }

        if (shifts == 8) {
            putc(byte, outptr);
            shifts = 0;
            byte = 0x00;
        }
        i++;
        ch = encoding[i];
    }

    while (shifts != 8) {
        byte >>= 1;
        shifts++;
    }

    fputc(byte, outptr);

    fclose(outptr);
    
    return ENCODE_SUCCESS;
}