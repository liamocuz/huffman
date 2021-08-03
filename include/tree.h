// Struct Definitions
typedef struct Node {
    struct Node* left;
    struct Node* right;
    int weight;
    char c;
} Node;

typedef struct List {
    struct Node* node;
    struct List* next;
} List;

// Function Declarations
void sortTreeArray(List* head);
void swap(Node* a, Node* b);
void printArr(List* head);
void freeArr(List* head);
Node* joinNodes(Node* a, Node* b);
void freeTree(Node* root);
void preOrderPrint(Node* node);
void preOrderTraversal(Node* node, char* encoding);
List* buildTreeFromList(List* head);

// Table functions
int buildTableFromTree(Node* root, char** table);
void initTable(char** table);
void printTable(char** table);
void freeTable(char** table);