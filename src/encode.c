#include "header.h"
#include "encode.h"

int compress(char* input, char* output) {
    int asciiCount[ASCII_SIZE];     // Stores the count of each ascii character in the input file
    char topology[TOPOLOGY_SIZE];   // Memory space for the topology
    char* table[ASCII_SIZE];        // This table is used to store the string encoding for each unique character
    Node* root = NULL;              // The root of the tree constructed from the huffman codes of each character
    Header header;                  // The header struct to be placed at the beginning of the beginning of the output file (24 bytes)
    header.numCharsComp = 0;        // Number of bytes that the compressed text takes up
    header.numCharsTopology = 0;    // Number of unique characters storing the topology
    header.numCharsUncomp = 0;      // Number of total characters in the input file

    /*
    The compressed file will have this structure
    Header - 24 bytes
    Topology - n bytes
    Encoding - m bytes
    */

    // Make sure all counts are set to 0
    memset(asciiCount, 0, sizeof(asciiCount));
    memset(topology, 0, sizeof(topology));

    // Count the occurence of each ASCII character as well as total number of characters in the input file
    // Also gets the number of unique chars and total chars for the header info
    if ((countChars(input, asciiCount, &header.numCharsTopology, &header.numCharsUncomp)) != ENCODE_SUCCESS) {
        return ENCODE_FAILURE;
    }

    // Constructs a tree from the characters occurences, returns the root of the tree
    if ((root = createTree(header.numCharsTopology, asciiCount)) == NULL) {
        printf("Error: Could not build tree.\n");
        return ENCODE_FAILURE;
    }

    // Initializes the table with NULL memory values for each entry
    initTable(table);

    // Populates the array of ASCII_SIZE with a corresponding huffman string code for each unique character
    if (buildTableFromTree(root, table, topology, &header, asciiCount) != ENCODE_SUCCESS) {
        printf("Error: Could not create table.\n");
        return ENCODE_FAILURE;
    }

    printf("Topology print\n");
    printf("%s\n", topology);
    printf("Topology Size: %d\n", strlen(topology));

    // printf("Encode tree print\n");
    // preOrderPrint(root);

    // Need to correct for bits in the extra byte
    header.numCharsComp = header.numCharsComp % 8 == 0 ? header.numCharsComp / 8 : header.numCharsComp / 8 + 1;

    // DEBUG
    printf("numCharsComp: %li, numCharsTopology: %li, numCharsUncomp: %li\n",
        header.numCharsComp, header.numCharsTopology, header.numCharsUncomp);
    printTable(table);


    // Write the header info and topology to the output file
    if (writeHeader(&header, output) != ENCODE_SUCCESS) {
        printf("Error: Coudld not write the header.\n");
        return ENCODE_FAILURE;
    }

    if (writeTopology(topology, output) != ENCODE_SUCCESS) {
        printf("Error: Unable to write the topology.\n");
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
    // printf("unique: %ld\n", *uniqueChars);

    fclose(fptr);

    return ENCODE_SUCCESS;
}
// Can do this function better
int compressFile(char* infile, char* outfile, char** table) {
    FILE* inptr = NULL;
    FILE* outptr = NULL;
    char* code = NULL;
    char ch = 0;
    int i = 0;
    unsigned char byte = 0x00;
    int shifts = 0;

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
            if (ch == '1') {
                byte >>= 1;
                byte |= 0x80;
                shifts++;
            }
            else if (ch == '0') {
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

int writeHeader(Header* header, char* outFile) {
    FILE* outptr = NULL;

    if ((outptr = fopen(outFile, "w")) == NULL) {
        printf("Error: Unable to open file %s.\n", outFile);
        return ENCODE_FAILURE;
    }

    fwrite(header, sizeof(*header), 1, outptr);

    fclose(outptr);

    return ENCODE_SUCCESS;
}

int writeTopology (char* topology, char* output) {
    char ch = 0;
    int i = 0;
    int bit = 0;
    unsigned char byte = 0x00;
    const unsigned char masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    FILE* outptr = NULL;

    if ((outptr = fopen(output, "a")) == NULL) {
        printf("Error: Unable to open file %s.\n", output);
        return ENCODE_FAILURE;
    }

    // Go through each char one at a time
    // If '0', move bit forward
    // If '1', OR in a 1 from masks, then place a full 8-bits of the i+1 char
    // Make sure to check if bit == 8 anytime when it is incremented
    // When bit == 8, write that byte
    // 001g1o001s1 001e1h01p1r
    // 3c fb c6 b9 20 2c 8b 26 5c 39
    ch = topology[i];
    while (ch != '\0') {
        if (bit == 8) {
            fputc(byte, outptr);
            byte = 0x00;
            bit = 0;
        }

        if (ch == '0') {
            bit++;            
        }
        else if (ch == '1') {
            byte |= masks[bit];
            bit++;

            i++;
            ch = topology[i];

            if (bit == 8) {
                fputc(byte, outptr);
                fputc(ch, outptr);
                byte = 0x00;
                bit = 0;
            }
            else {
                byte |= ch << bit;
                fputc(byte, outptr);
                byte = 0x00 | (ch >> (8 - bit));
            }
        }
        else {
            printf("Error: ASCII character not handled properly when writing topology.\n");
            return ENCODE_FAILURE;
        }

        i++;
        ch = topology[i];
    }

    if (byte) {
        // printf("putting: %x\n", byte);
        fputc(byte, outptr);
    }

    fclose(outptr);
    
    return ENCODE_SUCCESS;
}
