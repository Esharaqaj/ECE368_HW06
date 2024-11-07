#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct Node {
    char label;   // Node label: 'H', 'V', or ASCII representation for a number
    int id;
    int width;
    int height;
    int x_origin;
    int y_origin;
    struct Node *left_child;
    struct Node *right_child;
} Node;

typedef struct Stack {
    Node *tree_node;
    struct Stack *next;
} Stack;

Stack *createTreeFromFile(const char *filename);
void releaseStack(Stack *stack);
void displayNode(Node *root, FILE *print_file, FILE *dim_file, FILE *pck_file);
void calculateNodeCoordinates(Node *root);
Node *createNodeFromString(const char *buffer);
void addNodeToStack(Stack **stack, Node *node);
Node *popNodeFromStack(Stack **stack);
Node *constructSubtree(Stack **stack, const char *buffer);
void releaseTree(Node *root);