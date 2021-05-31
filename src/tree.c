#include "tree.h"
#include "header.h"

// Sort the array using selection sort
void sortTreeArray(Node* arr, int size) {
    int i =0;
    int j = 0;
    int mindex = 0;

    for (i = 0; i < size - 1; i++) {
        mindex = i;
        for (j = i + 1; j < size; j++) {
            if (arr[j].weight < arr[mindex].weight) {
                mindex = j;
            }
            else if (arr[j].c == -1 && arr[mindex].c != -1) {
                mindex = j;
            }
        }

        swap(&arr[mindex], &arr[i]);
    }    
}

void swap(Node* a, Node* b)  
{  
    Node temp = *a;
    *a = *b;  
    *b = temp;  
}

void printArr(Node* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("char: %c, weight: %d\n", arr[i].c, arr[i].weight);
    }
}