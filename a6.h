#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int checker;
    int identity; 
    int width;
    int height;
    int x, y; 
    char direction; 
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* buildTree(FILE *inputFile);
void preorderTraversal(TreeNode *node, FILE *out);
void findDirection(TreeNode *node, FILE *out);
void findXY(TreeNode *node, int x, int y, FILE *out);
void deleteTree(TreeNode *node);