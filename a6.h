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
    int x_co;
    int y_co;
    struct Node *leftNode;
    struct Node *rightNode;
} Node;

typedef struct Stack {
    struct Node *node;
    struct Stack *next;
} Stack;

// Function Prototypes with Renamed Functions

void printNode(Node *root, FILE *out1, FILE *out2, FILE *out3);
Stack* buildTree(const char *filename);
void deleteStack(Stack *stack_head);
Node* popStack(Stack **stack_head);
void findXY(Node *root);
Node* createNode(const char *buffer);
void deleteTree(Node *root);
Node* buildSmall(Stack **stack_head, const char *buffer);
void pushStack(Stack **stack_head, Node *new_node);