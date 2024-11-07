#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "a6.h"

Node* popStack(Stack **stack_head) {
    if (*stack_head == NULL) {
        return NULL;
    }
    Stack *temp = *stack_head;
    *stack_head = (*stack_head)->next;
    Node *popped_node = temp->node;
    free(temp);
    return popped_node;
}

Node* createNode(const char *buffer) {
    int id, width, height;
    sscanf(buffer, "%d(%d,%d)", &id, &width, &height);
    Node *new_node = malloc(sizeof(Node));
    new_node->int_name = id;
    new_node->node_name = '\0';
    new_node->width = width;
    new_node->height = height;
    new_node->x_co = 0;
    new_node->y_co = 0;
    new_node->leftNode = NULL;
    new_node->rightNode = NULL;
    return new_node;
}

void pushStack(Stack **stack_head, Node *new_node) {
    Stack *new_stack_node = malloc(sizeof(Stack));
    if (new_stack_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_stack_node->node = new_node;
    new_stack_node->next = *stack_head;
    *stack_head = new_stack_node;
}

void deleteTree(Node *root) {
    if (root == NULL) return;
    deleteTree(root->leftNode);
    deleteTree(root->rightNode);
    free(root);
}

void deleteStack(Stack *stack_head) {
    while (stack_head != NULL) {
        Stack *next_node = stack_head->next;
        if (stack_head->node != NULL) {
            deleteTree(stack_head->node);
        }
        free(stack_head);
        stack_head = next_node;
    }
}

Node* buildSmall(Stack **stack_head, const char *buffer) {
    char node_type;
    sscanf(buffer, "%c", &node_type);

    Node *new_node = malloc(sizeof(Node));
    new_node->node_name = node_type;
    new_node->int_name = 0;

    new_node->rightNode = popStack(stack_head);
    new_node->leftNode = popStack(stack_head);

    if (node_type == 'V') {
        new_node->height = (new_node->leftNode->height > new_node->rightNode->height) ? new_node->leftNode->height : new_node->rightNode->height;
        new_node->width = new_node->leftNode->width + new_node->rightNode->width;
    } else if (node_type == 'H') {
        new_node->width = (new_node->leftNode->width > new_node->rightNode->width) ? new_node->leftNode->width : new_node->rightNode->width;
        new_node->height = new_node->leftNode->height + new_node->rightNode->height;
    } else {
        new_node->width = 0;
        new_node->height = 0;
    }

    new_node->x_co = 0;
    new_node->y_co = 0;
    return new_node;
}

Stack* buildTree(const char *filename) {
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
            new_node = buildSmall(&stack_head, buffer);
        } else {
            new_node = createNode(buffer);
        }
        pushStack(&stack_head, new_node);
    }
    fclose(file);
    return stack_head;
}

void findXY(Node *root) {
    if (root->leftNode == NULL && root->rightNode == NULL) {
        return;
    }

    if (root->node_name == 'H') {
        root->leftNode->y_co = root->y_co + root->rightNode->height;
        root->leftNode->x_co = root->x_co;
        root->rightNode->x_co = root->x_co;
        root->rightNode->y_co = root->y_co;
    } else if (root->node_name == 'V') {
        root->leftNode->x_co = root->x_co;
        root->leftNode->y_co = root->y_co;
        root->rightNode->x_co = root->x_co + root->leftNode->width;
        root->rightNode->y_co = root->y_co;
    }

    findXY(root->leftNode);
    findXY(root->rightNode);
}

void printNode(Node *root, FILE *out1, FILE *out2, FILE *out3) {
    if (root == NULL) return;

    if (root->leftNode == NULL && root->rightNode == NULL) {
        fprintf(out1, "%d(%d,%d)\n", root->int_name, root->width, root->height);
    } else {
        fprintf(out1, "%c\n", root->node_name);
    }

    printNode(root->leftNode, out1, out2, out3);
    printNode(root->rightNode, out1, out2, out3);

    if ((root->node_name == 'V' || root->node_name == 'H') && root->int_name == 0) {
        fprintf(out2, "%c(%d,%d)\n", root->node_name, root->width, root->height);
    } else {
        fprintf(out2, "%d(%d,%d)\n", root->int_name, root->width, root->height);
        fprintf(out3, "%d((%d,%d)(%d,%d))\n", root->int_name, root->width, root->height, root->x_co, root->y_co);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_file> <output_file1> <output_file2> <output_file3>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Stack *treeStack = buildTree(argv[1]);
    if (treeStack == NULL) {
        fprintf(stderr, "Failed to build tree from input file.\n");
        return EXIT_FAILURE;
    }

    Node *root = popStack(&treeStack);
    deleteStack(treeStack);

    findXY(root);

    FILE *out1 = fopen(argv[2], "w");
    FILE *out2 = fopen(argv[3], "w");
    FILE *out3 = fopen(argv[4], "w");

    if (!out1 || !out2 || !out3) {
        perror("Error opening output file");
        deleteTree(root);
        if (out1) fclose(out1);
        if (out2) fclose(out2);
        if (out3) fclose(out3);
        return EXIT_FAILURE;
    }

    printNode(root, out1, out2, out3);

    fclose(out1);
    fclose(out2);
    fclose(out3);
    deleteTree(root);

    return EXIT_SUCCESS;
}