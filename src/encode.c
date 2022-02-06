#include "huffman.h"

int compress(char* input, char* output) {
    Header header;                  // The header struct to be placed at the beginning of the beginning of the output file (24 bytes)
    int asciiCount[ASCII_SIZE];     // Stores the count of each ascii character in the input file
    char* table[ASCII_SIZE];        // This table is used to store the string encoding for each unique character
    char gather[ASCII_SIZE];        // Memory space for gathering the huffman code string
    char* topology = NULL;          // Memory space for the topology
    Node* root = NULL;              // The root of the tree constructed from the huffman codes of each ch

    // Used for the preOrderTraversal function 
    int gatherSize = 0;             // Start the gather size of the string at 0
    int topologySize = 0;           // Start the topology size of the string at 0aracter

    header.numCharsComp = 0;        // Number of bytes that the compressed text takes up
    header.numCharsTopology = 0;    // Number of unique characters storing the topology
    header.numCharsUncomp = 0;      // Number of total characters in the input file
     
    /*
    The compressed file will have this structure
    Header - 24 bytes
    Topology - n bytes
    Encoding - m bytes
    */

    // Make sure all values are initialized to 0
    memset(asciiCount, 0, sizeof(asciiCount));
    memset(gather, 0, sizeof(gather));

    // Count the occurence of each ASCII character as well as total number of characters in the input file
    // Also gets the number of unique chars and total chars for the header info
    if ((countChars(input, asciiCount, &header.numCharsTopology, &header.numCharsUncomp)) != ENCODE_SUCCESS) {
        return ENCODE_FAILURE;
    }

    // Dynamically allocate space for topology
    topology = (char*)malloc(8 * header.numCharsTopology * sizeof(char));
    if (topology == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return ENCODE_FAILURE;
    }
    memset(topology, 0, 8 * header.numCharsTopology * sizeof(char));

    // Constructs a tree from the characters occurences, returns the root of the tree
    if ((root = createTree(header.numCharsTopology, asciiCount)) == NULL) {
        printf("Error: Could not build tree.\n");
        return ENCODE_FAILURE;
    }

    // Initializes the table with NULL memory values for each entry
    initTable(table);

    // Populates the huffman code table, pre order traversal topology, and calculates size of compressed text part
    preOrderTraversal(root, gather, &gatherSize, table, topology, &topologySize, asciiCount, &(header.numCharsComp));

    // Need to correct for bits in the extra byte
    header.numCharsComp = header.numCharsComp % 8 == 0 ? header.numCharsComp / 8 : header.numCharsComp / 8 + 1;

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

    free(topology);

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

    fclose(fptr);

    return ENCODE_SUCCESS;
}

// Creates a huffman coding tree from the asciiCount
Node* createTree(long int uniqueChars, int* asciiCount) {
    int i = 0;              // An iterator used to count from 0 to ASCII_SIZE
    List* head = NULL;      // Head of the linked list that contains a tree node
    List* ltemp = NULL;     // Temp variable to track previous List entry
    Node* ntemp = NULL;     // Temp variable of a new node being created for the Node member of the List
    Node* root = NULL;      // The final root of the tree

    for (i = 0; i < ASCII_SIZE; i++) {
        if (asciiCount[i] != 0) {
            if (head == NULL) {
                head = (List*)malloc(sizeof(List));

                ntemp = (Node*)malloc(sizeof(Node));
                ntemp->c =(char)i;
                ntemp-> weight = asciiCount[i];
                ntemp->left = NULL;
                ntemp->right = NULL;

                ltemp = head;
                ltemp->node = ntemp;
                ltemp->next = NULL;
            }
            else {
                ltemp->next = (List*)malloc(sizeof(List));
                ltemp = ltemp->next;

                ntemp = (Node*)malloc(sizeof(Node));
                ntemp->c = (char)i;
                ntemp-> weight = asciiCount[i];
                ntemp->left = NULL;
                ntemp->right = NULL;

                ltemp->node = ntemp;
                ltemp->next = NULL;
            }
        }
    }

    // Does a selection sort on the Linked List based upon the weights of each List
    sortTreeArray(head);

    // Constructs the tree now from the linked list
    head = buildTreeFromList(head);
    root = head->node;
    free(head);

    return root;
}

// Sort the linked list using selection sort
void sortTreeArray(List* head) {
    List* i = head;
    List* j = NULL;
    List* min = NULL;

    while (i) {
        min = i;
        j = i->next;
        while (j) {
            if (j->node->weight < min->node->weight) {
                min = j;
            }
            j = j->next;
        }
        swap(min->node, i->node);
        i = i->next;
    }
}

// Builds the tree from the sorted linked list
List* buildTreeFromList(List* head) {
    List* curr = head;
    List* newHead = head;

    // Shrinks the list down one List node at a time until just one node is left with the tree root inside of it
    while (newHead->next) {
        curr->next->node = joinNodes(curr->node, curr->next->node);
        newHead = curr->next;
        free(curr);
        curr = newHead;

        // Places the new node in the correct spot based upon it's weight, just by swapping the 
        // Tree nodes inside of the list nodes. If new node is the same weight after current node(s), 
        // place it after them
        while (curr->next && (curr->node->weight >= curr->next->node->weight)) {
            swap(curr->node, curr->next->node);
            curr = curr->next;
        }

        // Returns curr back to the beginning of the list
        curr = newHead;
    }

    // Returns the final List node
    return newHead;
}

// Creates a new Node from two nodes
Node* joinNodes(Node* a, Node* b) {
    Node* new = (Node*)malloc(sizeof(Node));
    new->c = -1;
    new->weight = a->weight + b->weight;
    new->left = a;
    new->right = b;
    return new;
}

// Simple swap function for Tree nodes
void swap(Node* a, Node* b)  
{  
    Node temp = *a;
    *a = *b;  
    *b = temp;  
}

// Inits the table, which is an array of char arrays
void initTable(char** table) {
   int i = 0;
   for (i = 0; i < ASCII_SIZE; i++) {
       table[i] = NULL;
   }
}

// Does a pre order traversal of the huffman code tree
void preOrderTraversal(Node* node, char* gather, int* gatherSize, char** table, char* topology, int* topologySize, int* asciiCount, long int* numCharsComp) {
    /*
    gather is used to create a huffman code for each ascii char in the tree
    topology is used to store the pre order traversal topology of the tree
    numCharsComp tracks how many characters will be in the compressed file
    */
    if (!node) {
        return;
    }
    
    if (!(node->left) && !(node->right)) {
        // If at leaf node, concatenate a '1' and char to topology
        topology[(*topologySize)] = '1';
        (*topologySize)++;
        topology[(*topologySize)] = node->c;
        (*topologySize)++;

        // If at leaf node, write the gather to the index in the table
        table[(int)node->c] = (char*)malloc(sizeof(char) * (*gatherSize) + 1);
        strcpy(table[(int)node->c], gather);
        (*numCharsComp) += (*gatherSize) * asciiCount[(int)node->c];

        // About to return up a level, shrink gather by 1 and decrement size
        gather[(*gatherSize) - 1] = '\0';
        (*gatherSize)--;

        return;
    }
    else {
        topology[(*topologySize)] = '0';
        (*topologySize)++;
    }

    if (node->left) {
        // Going left, cat a '0' to gather
        gather[(*gatherSize)] = '0';
        (*gatherSize)++;
        preOrderTraversal(node->left, gather, gatherSize, table, topology, topologySize, asciiCount, numCharsComp);
    }

    if (node->right) {
        // Going right, cat a '1' to gather
        gather[(*gatherSize)] = '1';
        (*gatherSize)++;
        preOrderTraversal(node->right, gather, gatherSize, table, topology, topologySize, asciiCount, numCharsComp);
    }
    
    // About to return up a level, shrink gather by 1 and decrement size
    gather[(*gatherSize) - 1] = '\0';
    (*gatherSize)--;

    return;
}

// Writes the header struct to the output file
int writeHeader(Header* header, char* outFile) {
    FILE* outptr = NULL;

    if ((outptr = fopen(outFile, "w")) == NULL) {
        printf("Error: Unable to open file %s.\n", outFile);
        return ENCODE_FAILURE;
    }

    if (fwrite(header, sizeof(*header), 1, outptr) != 1) {
        printf("Error: Did not write correct amount of bytes.\n");
        return ENCODE_FAILURE;
    }

    fclose(outptr);

    return ENCODE_SUCCESS;
}

// Writes and compresses the topology to the output file
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
        fputc(byte, outptr);
    }

    fclose(outptr);
    
    return ENCODE_SUCCESS;
}

// Compresses the file by converting each character into a huffman code and writes to the output file
int compressFile(char* infile, char* outfile, char** table) {
    FILE* inptr = NULL;             // File pointer for input file
    FILE* outptr = NULL;            // File pointer for output file
    char* code = NULL;              // Grabs the huffman code string from the table
    char ch = 0;                    // Grabs the char from the input file
    int i = 0;                      // Iterates through the huffman code
    unsigned char byte = 0x00;      // 8 bits so that byte by byte writing can be done
    int shifts = 0;                 // Tracks how many bits have been shifted into byte

    // Get input and output file pointers
    if ((inptr = fopen(infile, "r")) == NULL) {
        printf("Error: Unable to open file %s.\n", infile);
        return ENCODE_FAILURE;
    }

    if ((outptr = fopen(outfile, "a")) == NULL) {
        printf("Error: Unable to open file %s.\n", outfile);
        return ENCODE_FAILURE;
    }

    // Read through entire input file
    // For each character, get the corresponding code from the huffman table and shift into byte
    // Write a byte once 8 bits have been shifted into byte
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

/*
    Helper Functions
*/

// Functions to free dynamically allocated memory
void freeArr(List* head) {
    List* temp = head;
    List* next = NULL;

    while(temp) {
        next = temp->next;
        free(temp->node);
        free(temp);
        temp = next;
    }
}

void freeTree(Node* node) {
    if (node == NULL)
		return;

	freeTree(node->left);
	freeTree(node->right);

	free(node);
}

void freeTable(char** table) {
    int i = 0;

    for (i = 0; i < ASCII_SIZE; i++) {
        if (table[i]) {
            free(table[i]);
        }
    }
}

// Functions to print for debugging
void printArr(List* head) {
    List* runner = head;
    while (runner) {
        if (runner->node->c == -1) {
            preOrderPrint(runner->node);
        }
        else {
            printf("char: %c, weight: %d\n", runner->node->c, runner->node->weight);
        }
        runner = runner->next;
    }

    printf("\n");
}

void preOrderPrint(Node* node)
{
	if (node == NULL)
		return;
	printf("char: %c, weight: %d\n", node->c, node->weight);
	preOrderPrint(node->left);
	preOrderPrint(node->right);
}

void printTable(char** table) {
    int i = 0;
    for (i = 0; i < ASCII_SIZE; i++) {
        if (table[i] != NULL) {
            printf("%c: %s\n", (char)i, table[i]);
        }
    }
}
