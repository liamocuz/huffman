#include "encode.h"
#include "header.h"

int decode(char* input, char* output) {
    Header header;
    Node* root = NULL;
    FILE* inptr = NULL;
    unsigned long int fsize = 0;
    unsigned char byteEncoding[ENCODING_SIZE];
    unsigned char strEncoding[ENCODING_SIZE];
    unsigned char pathEncoding[ENCODING_SIZE];

    memset(byteEncoding, 0, sizeof(byteEncoding));
    memset(strEncoding, 0, sizeof(strEncoding));    // Used for debugging
    memset(&header, 0, sizeof(header));

    if ((inptr = fopen(input, "r")) == NULL) {
        printf("Unable to open file %s.\n", input);
        return ENCODE_FAILURE;
    }

    fseek(inptr, 0, SEEK_END);
    fsize = ftell(inptr);
    fseek(inptr, 0, SEEK_SET);
    printf("fsize: %lu\n", fsize);

    fread(&header, sizeof(header), 1, inptr);
    printf("ftell1: %lu\n", ftell(inptr));
    fread(byteEncoding, sizeof(unsigned char), header.numCharsComp, inptr);
    printf("ftell2: %lu\n", ftell(inptr));
    printf("ftell diff: %lu\n", fsize - ftell(inptr));
    fread(pathEncoding, sizeof(unsigned char), fsize - ftell(inptr), inptr);
    printf("ftell3: %lu\n", ftell(inptr));

    parseByteEncoding(byteEncoding, strEncoding, header.numCharsTopology);

    // DEBUG
    printf("numCharsComp: %li, numCharsTopology: %li, numCharsUncomp: %li\n",
        header.numCharsComp, header.numCharsTopology, header.numCharsUncomp);
    printf("Byte encoding: %s\n", byteEncoding);
    printf("Byte encoding length: %lu\n", strlen(byteEncoding));
    printf("String encoding: %s\n", strEncoding);
    printf("String encoding length: %lu\n", strlen(strEncoding));
    printf("Path encoding: %s\n", pathEncoding);
    printf("Path encoding length: %lu\n", strlen(pathEncoding));

    int k = 0;
    unsigned char ch;
    while ((ch = pathEncoding[k]) != '\0') {
        printf("%x", ch);
        k++;
    }
    printf("\n");

    int i = 0;
    int numCharsPut = 0;
    root = (Node*)malloc(1 * sizeof(Node));
    initNode(root);
    buildTreeFromEncoding(root, strEncoding, &i, header.numCharsTopology, &numCharsPut);

    preOrderPrint(root);

    if (decompress(output, root, pathEncoding, header.numCharsUncomp) != ENCODE_SUCCESS) {
        printf("Error: Could not decompress file\n");
        return ENCODE_FAILURE;
    }

    fclose(inptr);
    freeTree(root);

    return ENCODE_SUCCESS;
}

void buildTreeFromEncoding(Node* node, unsigned char* encoding, int* i, long int numCharsTopology, int* numCharsPut) {
    // Preorder: Visit root, visit left, visit right
    if (*numCharsPut == numCharsTopology) {
        printf("Returning from buildTreeFromEncoding\n");
        return;
    }

    if (encoding[*i] == '0') {
        node->left = (Node*)malloc(1 * sizeof(Node));
        initNode(node->left);
        (*i)++;
        buildTreeFromEncoding(node->left, encoding, i, numCharsTopology, numCharsPut);
        node->right = (Node*)malloc(1 * sizeof(Node));
        initNode(node->right);
        (*i)++;
        buildTreeFromEncoding(node->right, encoding, i, numCharsTopology, numCharsPut);
    }
    else if (encoding[*i] == '1') {
        node->c = encoding[*i + 1];
        (*i)++;
        (*numCharsPut)++;
    }

    return;
}

void initNode(Node* node) {
    node->left = NULL;
    node->right = NULL;
    node->weight = 0;
    node->c = -1;
    return;
}


void parseByteEncoding(unsigned char* byteEncoding, unsigned char* strEncoding, long int numCharsTopology) {
    int bit = 0;
    int charBit = 0;
    int charFlag = 0;
    int numCharsPut = 0;
    int i = 0;
    int j = 0;
    const char zero = '0';
    const char one = '1';
    const unsigned char masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    unsigned char byte = 0x00;
    unsigned char charByte = 0x00;

    while (numCharsPut != numCharsTopology) {
        while (bit < 8) {
            byte = byteEncoding[i];
            if (charFlag) {
                byte <<= charBit;
                charByte |= byte;
                bit += (7 - charBit);
                charBit = 8;
                charFlag = 0;
            }
            else {
                byte &= masks[bit];
                byte >>= bit;

                if (byte == 0x00) {
                    strEncoding[j++] = zero;
                }
                else if (byte == 0x01) {
                    strEncoding[j++] = one;
                    
                    // New char
                    charFlag = 1;
                    byte = byteEncoding[i];
                    charByte = (byte >> (bit + 1));
                    charBit = (7 - bit);
                    bit = 7;
                }
            }

            if (charBit == 8) {
                printf("putting: %c\n", charByte);
                strEncoding[j++] = charByte;
                numCharsPut++;
                charBit = 0;
                charByte = 0x00;
            }

            if (numCharsPut == numCharsTopology) {
                printf("returning\n");
                return;
            }

            bit++;
        }

        bit = 0;
        i++;
    }
}

int decompress(char* output, Node* root, unsigned char* encoding, long int numCharsUncomp) {
    FILE* outptr = NULL;
    Node* node = root;
    int numCharsPut = 0;
    int i = 0;
    int bit = 0;
    const unsigned char masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    unsigned char byte = 0x00;

    if ((outptr = fopen(output, "w")) == NULL) {
        printf("Error: Unable to open file %s.\n", output);
        return ENCODE_FAILURE;
    }

    while (numCharsPut < numCharsUncomp) {
        node = root;
        if (!node) {
            printf("Error: Encountered a null node");
            return ENCODE_FAILURE;
        }
        
        while (node && node->c == -1) {
            byte = encoding[i];  
            byte &= masks[bit];
            byte >>= bit;

            if (byte == 0x00) {
                node = node->left;
            }
            else if (byte == 0x01) {
                node = node->right;
            }
            else {
                printf("Error: Recieved wrong byte value: %x\n", byte);
                return ENCODE_FAILURE;
            }
            bit++;

            if (bit == 8) {
                bit = 0;
                i++;
            }
        }
        putc(node->c, outptr);
        numCharsPut++;
    }

    fclose(outptr);
    return ENCODE_SUCCESS;
}