#include "a6.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

TreeNode* buildTree(FILE *inputFile) {
    TreeNode *stack[1000];
    int top = -1;
    char line[50];

    while (fgets(line, sizeof(line), inputFile)) {
        if (line[0] == 'H' || line[0] == 'V') {
            TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
            node->checker = 0;
            node->direction = line[0];
            node->right = stack[top--];
            node->left = stack[top--];
            stack[++top] = node;
        } else {
            int id, width, height;
            sscanf(line, "%d(%d,%d)", &id, &width, &height);
            TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
            node->checker = 1;
            node->identity = id;
            node->width = width;
            node->height = height;
            node->left = node->right = NULL;
            stack[++top] = node;
        }
    }
    return stack[0];
}

void preorderTraversal(TreeNode *node, FILE *out) {
    if (!node) return;
    if (node->checker) {
        fprintf(out, "%d(%d,%d)\n", node->identity, node->width, node->height);
    } else {
        fprintf(out, "%c\n", node->direction);
    }
    preorderTraversal(node->left, out);
    preorderTraversal(node->right, out);
}

void findDirection(TreeNode *node, FILE *out) {
    if (!node) return;
    findDirection(node->left, out);
    findDirection(node->right, out);

    if (node->checker) {
        fprintf(out, "%d(%d,%d)\n", node->identity, node->width, node->height);
    } else {
        if (node->direction == 'H') {
            node->width = node->left->width > node->right->width ? node->left->width : node->right->width;
            node->height = node->left->height + node->right->height;
        } else {
            node->width = node->left->width + node->right->width;
            node->height = node->left->height > node->right->height ? node->left->height : node->right->height;
        }
        fprintf(out, "%c(%d,%d)\n", node->direction, node->width, node->height);
    }
}

void findXY(TreeNode *node, int x, int y, FILE *out) {
    if (!node) return;
    if (node->checker) {
        node->x = x;
        node->y = y;
        fprintf(out, "%d((%d,%d)(%d,%d))\n", node->identity, node->width, node->height, node->x, node->y);
    } else {
        if (node->direction == 'H') {
            findXY(node->left, x, y + node->right->height, out);
            findXY(node->right, x, y, out);
        } else {
            findXY(node->left, x, y, out);
            findXY(node->right, x + node->left->width, y, out);
        }
    }
}

void deleteTree(TreeNode *node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    free(node);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *out1 = fopen(argv[2], "w");
    FILE *out2 = fopen(argv[3], "w");
    FILE *out3 = fopen(argv[4], "w");

    if (!inputFile || !out1 || !out2 || !out3) {
        return 1;
    }

    TreeNode *root = buildTree(inputFile);
    fclose(inputFile);

    preorderTraversal(root, out1);
    findDirection(root, out2);
    findXY(root, 0, 0, out3);

    fclose(out1);
    fclose(out2);
    fclose(out3);

    deleteTree(root);
    return 0;
}