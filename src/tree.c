#include "tree.h"
#include "header.h"

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

// Used for debugging
void preOrderPrint(Node* node)
{
	if (node == NULL)
		return;
	printf("%c\n", node->c);
	preOrderPrint(node->left);
	preOrderPrint(node->right);
}

void preOrderTraversal(Node* node, char* encoding) {
    const char zero = '0';
    const char one = '1';
    if (node == NULL) {
        return;
    }

    if (node->c == -1) {
        strncat(encoding, &zero, 1);
    }
    else {
        strncat(encoding, &one, 1);
        strncat(encoding, &node->c, 1);
    }
    preOrderTraversal(node->left, encoding);
    preOrderTraversal(node->right, encoding);
}