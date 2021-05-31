// Struct Definitions
typedef struct Node {
    struct Node* left;
    struct Node* right;
    int weight;
    char c;
} Node;

// Function Declarations
void sortTreeArray(Node* arr, int size);
void swap(Node* a, Node* b);
void printArr(Node* arr, int size);