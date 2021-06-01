#include "tree.h"
#include "header.h"

// // Sort the linked list using selection sort
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
            else if (j->node->c == -1 && min->node->c != -1) {
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

void printArr(List* head) {
    List* runner = head;
    while (runner) {
        printf("char: %c, weight: %d\n", runner->node->c, runner->node->weight);
        runner = runner->next;
    }
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

Node* joinNodes(Node* a, Node* b) {
    Node* new = (Node*)malloc(sizeof(Node));
    new->c = -1;
    new->weight = a->weight + b->weight;
    new->left = a;
    new->right = b;
    return new;
}

void freeTree(Node* node) {
    if (node == NULL)
		return;

	freeTree(node->left);
	freeTree(node->right);

	free(node);
}

void preOrderPrint(Node* node)
{
	if (node == NULL)
		return;
	printf("%c\n", node->c);
	preOrderPrint(node->left);
	preOrderPrint(node->right);
}