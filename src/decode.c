#include "encode.h"
#include "header.h"

int decode(char* input, char* output) {
    Header header;
    Node* root = NULL;
    FILE* inptr = NULL;
    char encoding[ENCODING_SIZE];

    memset(encoding, 0, sizeof(encoding));

    if ((inptr = fopen(input, "r")) == NULL) {
        printf("Unable to open file %s.\n", input);
        return ENCODE_FAILURE;
    }

    fread(&header, sizeof(header), 1, inptr);
    fread(encoding, sizeof(char), header.numCharsEncoding, inptr);

    // DEBUG
    printf("numCharsEncoding: %li, numCharsTopology: %li, numCharsUncomp: %li\n",
        header.numCharsEncoding, header.numCharsTopology, header.numCharsUncomp);

    printf("Encoding: %s\n", encoding);

    root = buildTreeFromEncoding(encoding, header.numCharsTopology);

    fclose(inptr);

    return ENCODE_SUCCESS;
}

Node* buildTreeFromEncoding(char* encoding, long int numCharsTopology) {


}