#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "a6.h"

int main(int argc, char *argv[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    Stack *stack_head = (Stack *)malloc(sizeof(Stack));
    stack_head->tree_node = NULL;
    stack_head->next = NULL;

    char buffer[20];
    while (fgets(buffer, sizeof(buffer), file)) {
        Node *new_node;
        if (buffer[0] == 'V' || buffer[0] == 'H') {
            new_node = constructSubtree(&stack_head, buffer);
        } else {
            new_node = createNodeFromString(buffer);
        }
        addNodeToStack(&stack_head, new_node);
    }

    fclose(file);
    return stack_head;
}

Node *popNodeFromStack(Stack **stack) {
    if (*stack == NULL) {
        return NULL;
    }

    Stack *temp = (*stack)->next;
    Node *popped_node = temp->tree_node;
    (*stack)->next = temp->next;
    free(temp);

    return popped_node;
}

Node *createNodeFromString(const char *buffer) {
    int id = 0, w = 0, h = 0;
    sscanf(buffer, "%d(%d,%d)", &id, &w, &h);

    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->id = id;
    new_node->label = '\0';
    new_node->width = w;
    new_node->height = h;
    new_node->x_origin = 0;
    new_node->y_origin = 0;
    new_node->left_child = NULL;
    new_node->right_child = NULL;

    return new_node;
}

void addNodeToStack(Stack **stack, Node *node) {
    Stack *new_entry = (Stack *)malloc(sizeof(Stack));
    if (!new_entry) {
        return;
    }

    new_entry->tree_node = node;
    new_entry->next = (*stack)->next;
    (*stack)->next = new_entry;
}

Node *constructSubtree(Stack **stack, const char *buffer) {
    char label;
    sscanf(buffer, "%c", &label);

    Node *new_subtree = (Node *)malloc(sizeof(Node));
    new_subtree->label = label;
    new_subtree->id = 0;
    new_subtree->right_child = popNodeFromStack(stack);
    new_subtree->left_child = popNodeFromStack(stack);

    if (label == 'V') {
        new_subtree->width = new_subtree->left_child->width + new_subtree->right_child->width;
        new_subtree->height = (new_subtree->left_child->height > new_subtree->right_child->height) ? new_subtree->left_child->height : new_subtree->right_child->height;
    } else if (label == 'H') {
        new_subtree->width = (new_subtree->left_child->width > new_subtree->right_child->width) ? new_subtree->left_child->width : new_subtree->right_child->width;
        new_subtree->height = new_subtree->left_child->height + new_subtree->right_child->height;
    } else {
        new_subtree->width = 0;
        new_subtree->height = 0;
    }

    new_subtree->x_origin = 0;
    new_subtree->y_origin = 0;

    return new_subtree;
}

void calculateNodeCoordinates(Node *node) {
    if (!node->left_child && !node->right_child) {
        return;
    }

    node->left_child->x_origin = node->x_origin;
    node->left_child->y_origin = (node->label == 'H') ? node->y_origin + node->right_child->height : node->y_origin;

    node->right_child->x_origin = (node->label == 'V') ? node->x_origin + node->left_child->width : node->x_origin;
    node->right_child->y_origin = node->y_origin;

    calculateNodeCoordinates(node->left_child);
    calculateNodeCoordinates(node->right_child);
}

void displayNode(Node *node, FILE *print_file, FILE *dim_file, FILE *pck_file) {
    if (!node) {
        return;
    }

    if (!node->left_child && !node->right_child) {
        fprintf(print_file, "%d(%d,%d)\n", node->id, node->width, node->height);
    } else {
        fprintf(print_file, "%c\n", node->label);
    }

    displayNode(node->left_child, print_file, dim_file, pck_file);
    displayNode(node->right_child, print_file, dim_file, pck_file);

    if ((node->label == 'V' || node->label == 'H') && node->id == 0) {
        fprintf(dim_file, "%c(%d,%d)\n", node->label, node->width, node->height);
    } else {
        fprintf(dim_file, "%d(%d,%d)\n", node->id, node->width, node->height);
        fprintf(pck_file, "%d((%d,%d)(%d,%d))\n", node->id, node->width, node->height, node->x_origin, node->y_origin);
    }
}

void releaseTree(Node *root) {
    if (!root) {
        return;
    }
    releaseTree(root->left_child);
    releaseTree(root->right_child);
    free(root);
}

void releaseStack(Stack *stack) {
    Stack *current = stack->next;
    while (current) {
        Stack *next_entry = current->next;
        if (current->tree_node) {
            releaseTree(current->tree_node);
        }
        free(current);
        current = next_entry;
    }
}