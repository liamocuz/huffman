#include "encode.h"
#include "header.h"

int decode(char* input, char* output) {
    Header header;
    Node* root = NULL;
    FILE* inptr = NULL;
    unsigned long int fsize = 0;
    unsigned long int topologySize = 0;
    unsigned char byteTopology[ENCODING_SIZE];
    char stringTopology[ENCODING_SIZE];
    unsigned char compressedEncoding[ENCODING_SIZE];

    memset(byteTopology, 0, sizeof(byteTopology));
    memset(stringTopology, 0, sizeof(stringTopology));    // Used for debugging
    memset(compressedEncoding, 0, sizeof(compressedEncoding));
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
    topologySize = fsize - sizeof(header) - header.numCharsComp;
    fread(byteTopology, topologySize, 1, inptr);
    fread(compressedEncoding, header.numCharsComp, 1, inptr);

    parseByteTopology(byteTopology, stringTopology, header.numCharsTopology);

    // DEBUG
    printf("numCharsComp: %li, numCharsTopology: %li, numCharsUncomp: %li\n",
        header.numCharsComp, header.numCharsTopology, header.numCharsUncomp);
    printf("Byte topology size: %li\n", topologySize);

    // int k = 0;
    // unsigned char ch;
    // while ((ch = byteTopology[k]) != '\0') {
    //     printf("%x", ch);
    //     k++;
    // }
    // printf("\n");

    // printf("Byte topology: %s\n", byteTopology);
    printf("String topology size: %li\n", strlen(stringTopology));
    printf("String topology: %s\n", stringTopology);

    // k = 0;
    // while ((ch = compressedEncoding[k]) != '\0') {
    //     printf("%x", ch);
    //     k++;
    // }
    // printf("\n");

    int i = 0;
    int numCharsPut = 0;
    root = (Node*)malloc(sizeof(Node));
    initNode(root);
    buildTreeFromTopology(root, stringTopology, &i, header.numCharsTopology, &numCharsPut);

    // preOrderPrint(root);

    if (decompress(output, root, compressedEncoding, header.numCharsUncomp) != ENCODE_SUCCESS) {
        printf("Error: Could not decompress file\n");
        return ENCODE_FAILURE;
    }

    fclose(inptr);
    freeTree(root);

    return ENCODE_SUCCESS;
}

void buildTreeFromTopology(Node* node, char* topology, int* i, long int numCharsTopology, int* numCharsPut) {
    // Preorder: Visit root, visit left, visit right
    if (!node || *numCharsPut == numCharsTopology) {
        return;
    }
    
    if (topology[*i] == '1') {
        // printf("putting %c %d into tree\n", topology[*i + 1], (int)topology[*i + 1]);
        node->c = topology[*i + 1];
        (*i)++;
        (*numCharsPut)++;
        return;
    }
    
    node->left = (Node*)malloc(sizeof(Node));
    initNode(node->left);
    (*i)++;
    buildTreeFromTopology(node->left, topology, i, numCharsTopology, numCharsPut);

    node->right = (Node*)malloc(sizeof(Node));
    initNode(node->right);
    (*i)++;
    buildTreeFromTopology(node->right, topology, i, numCharsTopology, numCharsPut);

    return;
}

void initNode(Node* node) {
    node->left = NULL;
    node->right = NULL;
    node->weight = 0;
    node->c = -1;
    return;
}


void parseByteTopology(unsigned char* byteTopology, char* stringTopology, long int numCharsTopology) {
    int bit = 0;
    int charsPut = 0;
    int i = 0;
    int j = 0;
    const unsigned char masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    unsigned char byte = 0x00;
    unsigned char bitByte = 0x00;
    unsigned char charByte = 0x00;

    // This loop will process bytes until an entire char has been written
    while (charsPut != numCharsTopology) {
        if (bit == 8) {
            bit = 0;
            i++;
        }
        // printf("\nbeginning bit: %d\n", bit);
        byte = byteTopology[i];
        // printf("byte: %x\n", byte);

        bitByte = byte & masks[bit];
        bitByte >>= bit;
        bit++;

        while (bitByte == 0x00) {
            if (bit == 8) {
                bit = 0;
                i++;
                byte = byteTopology[i];
            }
            stringTopology[j++] = '0';
            bitByte = byte & masks[bit];
            bitByte >>= bit;
            bit++;
        }

        // A 1 was encountered, now read in 8 bits to charByte
        stringTopology[j++] = '1';
        // printf("charByte bit: %d\n", bit); 
        charByte = byte >> bit;
        i++;
        byte = byteTopology[i];
        charByte |= byte << (8 - bit);
        // printf("charByte %c %x, bit: %d\n", charByte, charByte, bit);

        stringTopology[j++] = charByte;
        charsPut++;
        printf("charsPut: %d charByte: %c\n", charsPut, charByte);
        charByte = 0x00;
        bitByte = 0x00;

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