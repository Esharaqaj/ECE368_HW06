#include "a6.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

TreeNode* createNode(char type, int label, int width, int height) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->type = type;
    node->width = width;
    node->height = height;
    node->x = node->y = 0;
    node->label = label;
    node->left = node->right = NULL;
    return node;
}

TreeNode* buildTreeFromPostOrder(FILE* inputFile) {
    char line[100];
    TreeNode *stack[1000];
    int top = -1;

    while (fgets(line, sizeof(line), inputFile)) {
        if (line[0] == 'H' || line[0] == 'V') {
            TreeNode *node = createNode(line[0], 0, 0, 0);
            node->right = stack[top--];
            node->left = stack[top--];
            stack[++top] = node;
        } else {
            int label, width, height;
            sscanf(line, "%d(%d,%d)", &label, &width, &height);
            stack[++top] = createNode(0, label, width, height);
        }
    }

    return stack[top];
}

void preOrderTraversal(TreeNode* node, FILE* outputFile) {
    if (!node) return;
    if (node->type == 0) {
        fprintf(outputFile, "%d(%d,%d)\n", node->label, node->width, node->height);
    } else {
        fprintf(outputFile, "%c\n", node->type);
    }
    preOrderTraversal(node->left, outputFile);
    preOrderTraversal(node->right, outputFile);
}

void computeDimensionsAndCoordinates(TreeNode* node) {
    if (!node) return;
    if (node->type == 0) return; 

    computeDimensionsAndCoordinates(node->left);
    computeDimensionsAndCoordinates(node->right);

    if (node->type == 'H') {
        node->width = (node->left->width > node->right->width) ? node->left->width : node->right->width;
        node->height = node->left->height + node->right->height;

        node->left->x = node->x;
        node->left->y = node->y + node->right->height;

        node->right->x = node->x;
        node->right->y = node->y;
    } else if (node->type == 'V') {
        node->width = node->left->width + node->right->width;
        node->height = (node->left->height > node->right->height) ? node->left->height : node->right->height;

        node->left->x = node->x;
        node->left->y = node->y;

        node->right->x = node->x + node->left->width;
        node->right->y = node->y;
    }
}

void outputDimensions(TreeNode* node, FILE* outputFile) {
    if (!node) return;
    if (node->type == 0) {
        fprintf(outputFile, "%d(%d,%d)\n", node->label, node->width, node->height);
    } else {
        fprintf(outputFile, "%c(%d,%d)\n", node->type, node->width, node->height);
    }
    outputDimensions(node->left, outputFile);
    outputDimensions(node->right, outputFile);
}

void outputCoordinates(TreeNode* node, FILE* outputFile) {
    if (!node) return;
    if (node->type == 0) {
        fprintf(outputFile, "%d((%d,%d)(%d,%d))\n", node->label, node->width, node->height, node->x, node->y);
    }
    outputCoordinates(node->left, outputFile);
    outputCoordinates(node->right, outputFile);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s in_file out_file1 out_file2 out_file3\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    TreeNode* root = buildTreeFromPostOrder(inputFile);
    fclose(inputFile);

    FILE* outFile1 = fopen(argv[2], "w");
    preOrderTraversal(root, outFile1);
    fclose(outFile1);

    FILE* outFile2 = fopen(argv[3], "w");
    computeDimensionsAndCoordinates(root);
    outputDimensions(root, outFile2);
    fclose(outFile2);

    FILE* outFile3 = fopen(argv[4], "w");
    outputCoordinates(root, outFile3);
    fclose(outFile3);

    deleteTree(root);
    

    return EXIT_SUCCESS;
}

void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    free(node);
}