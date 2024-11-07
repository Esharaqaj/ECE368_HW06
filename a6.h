#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct TreeNode {
    char label;
    int id;
    int width;
    int height;
    int x_coord;
    int y_coord;
    struct TreeNode *left_child;
    struct TreeNode *right_child;
} TreeNode;

typedef struct StackNode {
    struct TreeNode *tree;
    struct StackNode *next;
} StackNode;

StackNode *createTree(char *filename);
void freeStack(StackNode *stack_head);
void displayTree(TreeNode *root, FILE *output_pr, FILE *output_dim, FILE *output_pck);
void setCoordinates(TreeNode *root);
StackNode *createTree(char *filename);
TreeNode *assembleNode(StackNode **stack_head, char *buffer);
void freeTree(TreeNode *root);
void pushToStack(StackNode **stack_head, TreeNode *new_tree_node);
TreeNode *createNode(char *buffer);
TreeNode *popStack(StackNode **stack_head);