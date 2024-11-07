#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct TreeNode {
    char type;
    int width, height;
    int x, y;
    int label; 
    struct TreeNode *left, *right;
} TreeNode;

TreeNode* buildTree(FILE *inputFile);
void preorderTraversal(TreeNode *node, FILE *out);
void findDirection(TreeNode *node, FILE *out);
void findXY(TreeNode *node, int x, int y, FILE *out);
void deleteTree(TreeNode *node);