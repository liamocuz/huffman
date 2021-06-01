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
    List* temp = head;
    List* runner = head;
    List* newHead = head;
    List* toPlace = NULL;

    unsigned char end = 1;

    while (newHead->next) {
        toPlace = (List*)malloc(sizeof(List));
        toPlace->node = joinNodes(temp->node, temp->next->node);
        toPlace->next = NULL;

        if (temp->next->next) {
            newHead = temp->next->next;
        }
        else {
            newHead = toPlace;
            end = 0;
        }

        free(temp);
        free(temp->next);

        runner = newHead;
        while (runner->next && (runner->next->node->weight <= toPlace->node->weight)) {
            runner = runner->next;
        }

        if (runner == newHead && end) {
            newHead = toPlace;
            toPlace->next = runner;
        }
        else {
            insertListAfter(runner, toPlace);
        }

        temp = newHead;

        printArr(newHead);
    }


    return newHead;
}

void insertListAfter(List* after, List* toInsert) {
    List* temp = after->next;
    after->next = toInsert;
    toInsert->next = temp;
}

void printArr(List* head) {
    List* runner = head;
    while (runner) {
        printf("char: %c, weight: %d\n", runner->node->c, runner->node->weight);
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

void preOrderPrint(Node* node)
{
	if (node == NULL)
		return;
	printf("%c\n", node->c);
	preOrderPrint(node->left);
	preOrderPrint(node->right);
}