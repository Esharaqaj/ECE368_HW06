#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Struct Definitions
typedef struct Node {
    char node_name;
    int int_name;
    int width;
    int height;
    int org_x;
    int org_y;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Stack {
    struct Node *node;
    struct Stack *next;
} Stack;

// Function Prototypes with Renamed Functions
Stack* constructTree(const char *filename);
void disposeStack(Stack *stack_head);
void displayNode(Node *root, FILE *outFile, FILE *dimFile, FILE *packFile);
void calculateBoxCoords(Node *root);
Node* createNodeElement(const char *buffer);
Node* removeFromStack(Stack **stack_head);
void pushToStack(Stack **stack_head, Node *new_node);
void disposeTree(Node *root);
Node* buildSubtree(Stack **stack_head, const char *buffer);