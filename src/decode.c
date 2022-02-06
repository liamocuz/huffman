#include "huffman.h"

int decompress(char* input, char* output) {
    Header header;                                          // Header struct to store header info at beginning of input file
    Node* root = NULL;                                      // Root of the huffman tree to be constructed
    FILE* inptr = NULL;                                     // File pointer of input file
    unsigned long int fsize = 0;                            // File size of input file
    unsigned long int topologySize = 0;                     // The size in bytes of the pre order tree traversal topology
    unsigned char* byteTopology = NULL;                     // Memory to store the bytes of the pre order tree traversal from the input file
    char* stringTopology = NULL;                            // Memory to store the expanded pre order tree traversal in string form
    
    // These are used for the buildTreeFromTopology function
    int i = 0;
    int numCharsPut = 0;

    // Set all memory values to 0
    memset(&header, 0, sizeof(header));
    
    // Get input file pointer
    if ((inptr = fopen(input, "r")) == NULL) {
        printf("Unable to open file %s.\n", input);
        return ENCODE_FAILURE;
    }

    // Find the file length
    fseek(inptr, 0, SEEK_END);
    fsize = ftell(inptr);
    fseek(inptr, 0, SEEK_SET);
    
    // Dynamically allocate memory and read in to memory from file 
    if (fread(&header, sizeof(header), 1, inptr) != sizeof(header)) {
        printf("Error: Did not read in correct amount of bytes.\n");
        return ENCODE_FAILURE;
    }
    topologySize = fsize - sizeof(header) - header.numCharsComp;

    byteTopology = (unsigned char*)malloc(topologySize * sizeof(unsigned char));
    if (byteTopology == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return ENCODE_FAILURE;
    }
    if (fread(byteTopology, topologySize, 1, inptr) != topologySize) {
        printf("Error: Did not read in correct amount of bytes.\n");
        return ENCODE_FAILURE;
    }

    stringTopology = (char*)malloc(8 * topologySize * sizeof(char));
    if (stringTopology == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return ENCODE_FAILURE;
    }
    fclose(inptr);

    // Turns the byte topology into string form for easier processing
    parseByteTopology(byteTopology, stringTopology, header.numCharsTopology);

    // Build the huffman code tree
    root = (Node*)malloc(sizeof(Node));
    initNode(root);
    buildTreeFromTopology(root, stringTopology, &i, header.numCharsTopology, &numCharsPut);

    // Decompress the compressed text and write it to the output file
    if (decompressFile(input, sizeof(header) + topologySize, output, root, header.numCharsUncomp) != ENCODE_SUCCESS) {
        printf("Error: Could not decompress file.\n");
        return ENCODE_FAILURE;
    }

    freeTree(root);
    free(byteTopology);
    free(stringTopology);

    return ENCODE_SUCCESS;
}

// Turns the byte topology into a string topology
void parseByteTopology(unsigned char* byteTopology, char* stringTopology, long int numCharsTopology) {
    int bit = 0;        // Keeps track of the bit position
    int charsPut = 0;   // Keeps track of how many characters have been found
    int i = 0;          // Iterator for the byte topology
    int j = 0;          // Iterator for the string topology
    const unsigned char masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};    // An array of masks for each bit in a byte
    unsigned char byte = 0x00;      // Used to get each byte from byte topology
    unsigned char bitByte = 0x00;   // A byte that can be modified to check from byte
    unsigned char charByte = 0x00;  // The byte to read in a full 8-bit character

    // This loop will process bytes until an entire char has been written
    while (charsPut != numCharsTopology) {
        if (bit == 8) {
            bit = 0;
            i++;
        }
        byte = byteTopology[i];

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
        charByte = byte >> bit;
        i++;
        byte = byteTopology[i];
        charByte |= byte << (8 - bit);

        stringTopology[j++] = charByte;
        charsPut++;
        charByte = 0x00;
        bitByte = 0x00;
    }
}

// Creates the huffman tree from the string topology of the pre order traversal
void buildTreeFromTopology(Node* node, char* topology, int* i, long int numCharsTopology, int* numCharsPut) {
    // Preorder: Visit root, visit left, visit right
    if (!node || *numCharsPut == numCharsTopology) {
        return;
    }
    
    if (topology[*i] == '1') {
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

// Initializes a tree node
void initNode(Node* node) {
    node->left = NULL;
    node->right = NULL;
    node->weight = 0;
    node->c = -1;
    return;
}

// Decompresses the compressed characters from encoding and writes to the output file
int decompressFile(char* input, int offset, char* output, Node* root, long int numCharsUncomp) {
    FILE* inptr = NULL;             // Input file pointer
    FILE* outptr = NULL;            // Output file pointer
    Node* node = root;              // Node to traverse the tree from root
    int numCharsPut = 0;            // Track when same amount of characters from original file is put back
    int bit = 0;                    // Tracks bit position in byte
    const unsigned char masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // An array of masks for each bit in a byte
    unsigned char byte = 0x00;      // Byte used to read through encoding
    char charByte = 0x00;           // Byte used to grab char from input file compressed text

    // Open input and output files
    if ((inptr = fopen(input, "r")) == NULL) {
        printf("Error: Unable to open file %s.\n", input);
        return ENCODE_FAILURE;
    }
    fseek(inptr, offset, SEEK_SET); // Start reading from compressed text of original

    if ((outptr = fopen(output, "w")) == NULL) {
        printf("Error: Unable to open file %s.\n", output);
        return ENCODE_FAILURE;
    }

    // Read in initial byte 
    charByte = fgetc(inptr);
    while (numCharsPut < numCharsUncomp) {
        node = root;
        if (!node) {
            printf("Error: Encountered a null node.\n");
            return ENCODE_FAILURE;
        }
        
        // Traverse from root to leaf node and once there place that char to the output
        while (node && node->c == -1) {
            byte = charByte;
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
                charByte = fgetc(inptr); 
            }
        }

        fputc(node->c, outptr);
        numCharsPut++;
    }

    fclose(outptr);
    return ENCODE_SUCCESS;
}