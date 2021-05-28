// Struct definitions

typedef struct Node {
    struct Node* left;
    struct Node* right;
    int weight;
    char c;
} Node;