#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "a6.h"

Node* removeFromStack(Stack **stack_head) {
    if (*stack_head == NULL) {
        return NULL;
    }
    Stack *temp = *stack_head;
    *stack_head = (*stack_head)->next;
    Node *popped_node = temp->node;
    free(temp);
    return popped_node;
}

Node* createNodeElement(const char *buffer) {
    int id, width, height;
    sscanf(buffer, "%d(%d,%d)", &id, &width, &height);
    Node *new_node = malloc(sizeof(Node));
    new_node->int_name = id;
    new_node->node_name = '\0';
    new_node->width = width;
    new_node->height = height;
    new_node->org_x = 0;
    new_node->org_y = 0;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void pushToStack(Stack **stack_head, Node *new_node) {
    Stack *new_stack_node = malloc(sizeof(Stack));
    if (new_stack_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_stack_node->node = new_node;
    new_stack_node->next = *stack_head;
    *stack_head = new_stack_node;
}

void disposeTree(Node *root) {
    if (root == NULL) return;
    disposeTree(root->left);
    disposeTree(root->right);
    free(root);
}

void disposeStack(Stack *stack_head) {
    while (stack_head != NULL) {
        Stack *next_node = stack_head->next;
        if (stack_head->node != NULL) {
            disposeTree(stack_head->node);
        }
        free(stack_head);
        stack_head = next_node;
    }
}

Node* buildSubtree(Stack **stack_head, const char *buffer) {
    char node_type;
    sscanf(buffer, "%c", &node_type);

    Node *new_node = malloc(sizeof(Node));
    new_node->node_name = node_type;
    new_node->int_name = 0;

    new_node->right = removeFromStack(stack_head);
    new_node->left = removeFromStack(stack_head);

    if (node_type == 'V') {
        new_node->height = (new_node->left->height > new_node->right->height) ? new_node->left->height : new_node->right->height;
        new_node->width = new_node->left->width + new_node->right->width;
    } else if (node_type == 'H') {
        new_node->width = (new_node->left->width > new_node->right->width) ? new_node->left->width : new_node->right->width;
        new_node->height = new_node->left->height + new_node->right->height;
    } else {
        new_node->width = 0;
        new_node->height = 0;
    }

    new_node->org_x = 0;
    new_node->org_y = 0;
    return new_node;
}

Stack* constructTree(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    Stack *stack_head = NULL;
    char buffer[20];
    while (fgets(buffer, sizeof(buffer), file)) {
        Node *new_node;
        if (buffer[0] == 'V' || buffer[0] == 'H') {
            new_node = buildSubtree(&stack_head, buffer);
        } else {
            new_node = createNodeElement(buffer);
        }
        pushToStack(&stack_head, new_node);
    }
    fclose(file);
    return stack_head;
}

void calculateBoxCoords(Node *root) {
    if (root->left == NULL && root->right == NULL) {
        return;
    }

    if (root->node_name == 'H') {
        root->left->org_y = root->org_y + root->right->height;
        root->left->org_x = root->org_x;
        root->right->org_x = root->org_x;
        root->right->org_y = root->org_y;
    } else if (root->node_name == 'V') {
        root->left->org_x = root->org_x;
        root->left->org_y = root->org_y;
        root->right->org_x = root->org_x + root->left->width;
        root->right->org_y = root->org_y;
    }

    calculateBoxCoords(root->left);
    calculateBoxCoords(root->right);
}

void displayNode(Node *root, FILE *outFile, FILE *dimFile, FILE *packFile) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        fprintf(outFile, "%d(%d,%d)\n", root->int_name, root->width, root->height);
    } else {
        fprintf(outFile, "%c\n", root->node_name);
    }

    displayNode(root->left, outFile, dimFile, packFile);
    displayNode(root->right, outFile, dimFile, packFile);

    if ((root->node_name == 'V' || root->node_name == 'H') && root->int_name == 0) {
        fprintf(dimFile, "%c(%d,%d)\n", root->node_name, root->width, root->height);
    } else {
        fprintf(dimFile, "%d(%d,%d)\n", root->int_name, root->width, root->height);
        fprintf(packFile, "%d((%d,%d)(%d,%d))\n", root->int_name, root->width, root->height, root->org_x, root->org_y);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_file> <output_file1> <output_file2> <output_file3>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Stack *treeStack = constructTree(argv[1]);
    if (treeStack == NULL) {
        fprintf(stderr, "Failed to build tree from input file.\n");
        return EXIT_FAILURE;
    }

    Node *root = removeFromStack(&treeStack);
    disposeStack(treeStack);

    calculateBoxCoords(root);

    FILE *outputFile = fopen(argv[2], "w");
    FILE *dimensionsFile = fopen(argv[3], "w");
    FILE *packFile = fopen(argv[4], "w");

    if (!outputFile || !dimensionsFile || !packFile) {
        perror("Error opening output file");
        disposeTree(root);
        if (outputFile) fclose(outputFile);
        if (dimensionsFile) fclose(dimensionsFile);
        if (packFile) fclose(packFile);
        return EXIT_FAILURE;
    }

    displayNode(root, outputFile, dimensionsFile, packFile);

    fclose(outputFile);
    fclose(dimensionsFile);
    fclose(packFile);
    disposeTree(root);

    return EXIT_SUCCESS;
}