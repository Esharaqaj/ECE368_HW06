#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "a6.h"

TreeNode* popFromStack(NodeStack **stackHead) {
    if (*stackHead == NULL) {
        return NULL;
    }
    NodeStack *temp = *stackHead;
    *stackHead = (*stackHead)->next;
    TreeNode *poppedNode = temp->node;
    free(temp);
    return poppedNode;
}

TreeNode* createNode(const char *buffer) {
    int id, width, height;
    sscanf(buffer, "%d(%d,%d)", &id, &width, &height);
    TreeNode *newNode = malloc(sizeof(TreeNode));
    newNode->id = id;
    newNode->nodeType = '\0';
    newNode->width = width;
    newNode->height = height;
    newNode->posX = 0;
    newNode->posY = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void pushToStack(NodeStack **stackHead, TreeNode *newNode) {
    NodeStack *newStackNode = malloc(sizeof(NodeStack));
    if (newStackNode == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    newStackNode->node = newNode;
    newStackNode->next = *stackHead;
    *stackHead = newStackNode;
}

void releaseTree(TreeNode *root) {
    if (root == NULL) return;
    releaseTree(root->left);
    releaseTree(root->right);
    free(root);
}

void releaseStack(NodeStack *stackHead) {
    while (stackHead != NULL) {
        NodeStack *nextNode = stackHead->next;
        if (stackHead->node != NULL) {
            releaseTree(stackHead->node);
        }
        free(stackHead);
        stackHead = nextNode;
    }
}

TreeNode* createSubtree(NodeStack **stackHead, const char *buffer) {
    char nodeType;
    sscanf(buffer, "%c", &nodeType);

    TreeNode *newNode = malloc(sizeof(TreeNode));
    newNode->nodeType = nodeType;
    newNode->id = 0;

    newNode->right = popFromStack(stackHead);
    newNode->left = popFromStack(stackHead);

    if (nodeType == 'V') {
        newNode->height = (newNode->left->height > newNode->right->height) ? newNode->left->height : newNode->right->height;
        newNode->width = newNode->left->width + newNode->right->width;
    } else if (nodeType == 'H') {
        newNode->width = (newNode->left->width > newNode->right->width) ? newNode->left->width : newNode->right->width;
        newNode->height = newNode->left->height + newNode->right->height;
    } else {
        newNode->width = 0;
        newNode->height = 0;
    }
    newNode->posX = 0;
    newNode->posY = 0;
    return newNode;
}

NodeStack* createTree(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    NodeStack *stackHead = NULL;
    char buffer[20];
    while (fgets(buffer, sizeof(buffer), file)) {
        TreeNode *newNode;
        if (buffer[0] == 'V' || buffer[0] == 'H') {
            newNode = createSubtree(&stackHead, buffer);
        } else {
            newNode = createNode(buffer);
        }
        pushToStack(&stackHead, newNode);
    }
    fclose(file);
    return stackHead;
}

void calculateCoordinates(TreeNode *root) {
    if (root->left == NULL && root->right == NULL) {
        return;
    }

    if (root->nodeType == 'H') {
        root->left->posY = root->posY + root->right->height;
        root->right->posX = root->posX;
    } else if (root->nodeType == 'V') {
        root->left->posX = root->posX;
        root->right->posX = root->posX + root->left->width;
    }

    calculateCoordinates(root->left);
    calculateCoordinates(root->right);
}

void outputNodeData(TreeNode *root, FILE *outputFile, FILE *dimensionsFile, FILE *packFile) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        fprintf(outputFile, "%d(%d,%d)\n", root->id, root->width, root->height);
    } else {
        fprintf(outputFile, "%c\n", root->nodeType);
    }

    outputNodeData(root->left, outputFile, dimensionsFile, packFile);
    outputNodeData(root->right, outputFile, dimensionsFile, packFile);

    if ((root->nodeType == 'V' || root->nodeType == 'H') && root->id == 0) {
        fprintf(dimensionsFile, "%c(%d,%d)\n", root->nodeType, root->width, root->height);
    } else {
        fprintf(dimensionsFile, "%d(%d,%d)\n", root->id, root->width, root->height);
        fprintf(packFile, "%d((%d,%d)(%d,%d))\n", root->id, root->width, root->height, root->posX, root->posY);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_file> <output_file1> <output_file2> <output_file3>\n", argv[0]);
        return EXIT_FAILURE;
    }

    NodeStack *treeStack = createTree(argv[1]);
    if (treeStack == NULL) {
        fprintf(stderr, "Failed to build tree from input file.\n");
        return EXIT_FAILURE;
    }

    TreeNode *root = popFromStack(&treeStack);
    releaseStack(treeStack);

    calculateCoordinates(root);

    FILE *outputFile = fopen(argv[2], "w");
    FILE *dimensionsFile = fopen(argv[3], "w");
    FILE *packFile = fopen(argv[4], "w");

    if (!outputFile || !dimensionsFile || !packFile) {
        perror("Error opening output file");
        releaseTree(root);
        if (outputFile) fclose(outputFile);
        if (dimensionsFile) fclose(dimensionsFile);
        if (packFile) fclose(packFile);
        return EXIT_FAILURE;
    }

    outputNodeData(root, outputFile, dimensionsFile, packFile);

    fclose(outputFile);
    fclose(dimensionsFile);
    fclose(packFile);
    releaseTree(root);

    return EXIT_SUCCESS;
}