#include "tree.h"
#include "header.h"

// Returns tree root
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

    // DEBUG
    // printf("\nARRAY AFTER SORT\n\n");
    // printArr(head);

    // Constructs the tree now from the linked list
    head = buildTreeFromList(head);
    root = head->node;
    free(head);

    // DEBUG
    // printf("\nPREORDER PRINT\n\n");
    // preOrderPrint(root);
    // freeTree(root);

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

// Used for debugging
void preOrderPrint(Node* node)
{
	if (node == NULL)
		return;
	printf("char: %c, weight: %d\n", node->c, node->weight);
	preOrderPrint(node->left);
	preOrderPrint(node->right);
}

// 001g1o001s1 001e1h01p1r
/*
gather and topology cannot be treated the same
gather is if you go left or right
topology is '0' if non-leaf, '1' and char if lead node
size parameter is used to track the length of gather as it will need to grow and shrink
*/
void preOrderTraversal(Node* node, char* gather, int* gatherSize, char** table, char* topology, int* topologySize, int* asciiCount, long int* numCharsComp) {

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