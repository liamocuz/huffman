#include "tree.h"
#include "header.h"

// Returns tree root
Node* createTree(long int uniqueChars, int* asciiCount) {
    int i = 0;
    List* head = NULL;
    List* ltemp = NULL;
    Node* ntemp = NULL;
    Node* root = NULL;

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

    sortTreeArray(head);

    // DEBUG
    // printf("\nARRAY AFTER SORT\n\n");
    // printArr(head);

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

void swap(Node* a, Node* b)  
{  
    Node temp = *a;
    *a = *b;  
    *b = temp;  
}

Node* joinNodes(Node* a, Node* b) {
    Node* new = (Node*)malloc(sizeof(Node));
    new->c = -1;
    new->weight = a->weight + b->weight;
    new->left = a;
    new->right = b;
    return new;
}

List* buildTreeFromList(List* head) {
    List* curr = head;
    List* newHead = head;

    while (newHead->next) {
        curr->next->node = joinNodes(curr->node, curr->next->node);
        newHead = curr->next;
        free(curr);
        curr = newHead;

        while (curr->next && (curr->node->weight >= curr->next->node->weight)) {
            swap(curr->node, curr->next->node);
            curr = curr->next;
        }

        curr = newHead;

        // DEBUG
        // printArr(newHead);
    }

    return newHead;
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

void preOrderTraversal(Node* node, char* encoding, char* gather, char** table, long int* numCharsEncoding) {
    const char zero = '0';
    const char one = '1';
    if (node->c != -1) {
        strncat(encoding, &one, 1);
        strncat(encoding, &node->c, 1);
        (*numCharsEncoding)++;
        (*numCharsEncoding)++;
    }

    if (node->left) {
        strncat(encoding, &zero, 1);
        (*numCharsEncoding)++;
        strncat(gather, &zero, 1);
        preOrderTraversal(node->left, encoding, gather, table, numCharsEncoding);
    }
    
    if (node->right) {
        strncat(gather, &one, 1);
        preOrderTraversal(node->right, encoding, gather, table, numCharsEncoding);
    }
    
    // DEBUG
    // printf("char: %c, gather: %s\n", node->c, gather);

    if (!node->left && !node->right) {
        table[(int)node->c] = (char*)malloc(sizeof(char) * (strlen(gather) + 1));
        strcpy(table[(int)node->c], gather);
    }

    gather[strlen(gather) - 1] = 0;
    return;
}