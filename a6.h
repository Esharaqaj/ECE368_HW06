#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Struct Definitions
typedef struct TreeNode {
    char nodeType;   // ASCII representation for V, H, etc.
    int id;
    int width;
    int height;
    int posX;
    int posY;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct NodeStack {
    struct TreeNode *node;
    struct NodeStack *next;
} NodeStack;

// Function Prototypes
NodeStack* createTree(const char *filename);
void releaseStack(NodeStack *stackHead);
void outputNodeData(TreeNode *root, FILE *outputFile, FILE *dimensionsFile, FILE *packFile);
void calculateCoordinates(TreeNode *root);
NodeStack* createTree(const char *filename);
TreeNode* createSubtree(NodeStack **stackHead, const char *buffer);
void releaseTree(TreeNode *root);
void pushToStack(NodeStack **stackHead, TreeNode *newNode);
TreeNode* createNode(const char *buffer);
TreeNode* popFromStack(NodeStack **stackHead);