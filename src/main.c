#include "huffman.h"

int main (int argc, char** argv) {
    int mode = -1;
    char* input = NULL;
    char* output = NULL;

    if (argc != 4) {
        printf("Usage: <mode: 0 for compression, 1 for decompression> <input file> <output file>\n");
        return EXIT_FAILURE;
    }

    mode = atoi(argv[1]);
    input = argv[2];
    output = argv[3];

    if (mode == 0) {
        if (compress(input, output) == ENCODE_FAILURE) {
            printf("Exiting!\n");
            return EXIT_FAILURE;
        }
    }
    else if (mode == 1) {
        if (decode(input, output) == ENCODE_FAILURE) {
            printf("Exiting!\n");
            return EXIT_FAILURE;
        }
    }
    else {
        printf("Incorrect mode: %d\n", mode);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}