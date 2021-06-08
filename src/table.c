#include "header.h"
#include "encode.h"

int buildTableFromTree(Node* root, char** table) {
    char encoding[1024];
    char* gather = "";
    memset(encoding, 0, 1024);
    preOrderTraversal(root, encoding);

    // DEBUG
    // printf("%s\n", encoding);

    

    return ENCODE_SUCCESS;
}

