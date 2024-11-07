#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "a6.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_file> <output_pr_file> <output_dim_file> <output_pck_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read input file and build the tree
    StackNode *stack = createTree(argv[1]);
    if (stack == NULL) {
        fprintf(stderr, "Error reading input file.\n");
        return EXIT_FAILURE;
    }

    // Open output files
    FILE *output_pr = fopen(argv[2], "w");
    FILE *output_dim = fopen(argv[3], "w");
    FILE *output_pck = fopen(argv[4], "w");

    if (output_pr == NULL || output_dim == NULL || output_pck == NULL) {
        fprintf(stderr, "Error opening output files.\n");
        return EXIT_FAILURE;
    }

    // Get the root node from the stack
    TreeNode *root = stack->next->tree;

    // Set coordinates for all nodes
    setCoordinates(root);

    // Display tree details in the output files
    displayTree(root, output_pr, output_dim, output_pck);

    // Free memory and close files
    freeTree(root);
    freeStack(stack);
    fclose(output_pr);
    fclose(output_dim);
    fclose(output_pck);

    return EXIT_SUCCESS;
}


TreeNode *popStack(StackNode **stack_head) {
    if (*stack_head == NULL) {
        return NULL;
    }

    StackNode *temp = (*stack_head)->next;
    TreeNode *removed_node = temp->tree;
    (*stack_head)->next = temp->next;
    free(temp);

    return removed_node;
}

TreeNode *createNode(char *buffer) {
    int identifier = 0, width = 0, height = 0;
    sscanf(buffer, "%d(%d,%d)", &identifier, &width, &height);

    TreeNode *new_tree_node = malloc(sizeof(TreeNode));
    new_tree_node->id = identifier;
    new_tree_node->label = '\0';
    new_tree_node->width = width;
    new_tree_node->height = height;
    new_tree_node->x_coord = 0;
    new_tree_node->y_coord = 0;
    new_tree_node->left_child = NULL;
    new_tree_node->right_child = NULL;

    return new_tree_node;
}

void pushToStack(StackNode **stack_head, TreeNode *new_tree_node) {
    StackNode *new_stack_node = malloc(sizeof(StackNode));
    if (new_stack_node == NULL) {
        return;
    }

    new_stack_node->tree = new_tree_node;
    new_stack_node->next = (*stack_head)->next;
    (*stack_head)->next = new_stack_node;
}

void freeTree(TreeNode *root) {
    if (root == NULL) return;

    freeTree(root->left_child);
    freeTree(root->right_child);
    free(root);
}

void freeStack(StackNode *stack_head) {
    StackNode *current = stack_head->next;
    while (current != NULL) {
        StackNode *next_node = current->next;
        if (current->tree != NULL) {
            freeTree(current->tree);
        }
        free(current);
        current = next_node;
    }
}

TreeNode *assembleNode(StackNode **stack_head, char *buffer) {
    char label;
    sscanf(buffer, "%c", &label);

    TreeNode *new_internal_node = malloc(sizeof(TreeNode));
    new_internal_node->label = label;
    new_internal_node->id = 0;
    new_internal_node->right_child = popStack(stack_head);
    new_internal_node->left_child = popStack(stack_head);

    if (new_internal_node->label == 'V') {
        new_internal_node->height = (new_internal_node->left_child->height > new_internal_node->right_child->height) ?
                                    new_internal_node->left_child->height : new_internal_node->right_child->height;
        new_internal_node->width = new_internal_node->left_child->width + new_internal_node->right_child->width;
    } else if (new_internal_node->label == 'H') {
        new_internal_node->width = (new_internal_node->left_child->width > new_internal_node->right_child->width) ?
                                   new_internal_node->left_child->width : new_internal_node->right_child->width;
        new_internal_node->height = new_internal_node->left_child->height + new_internal_node->right_child->height;
    } else {
        new_internal_node->width = 0;
        new_internal_node->height = 0;
    }

    new_internal_node->x_coord = 0;
    new_internal_node->y_coord = 0;
    return new_internal_node;
}

StackNode *createTree(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    StackNode *stack_head = malloc(sizeof(StackNode));
    stack_head->tree = NULL;
    stack_head->next = NULL;
    char buffer[20];

    while (fgets(buffer, sizeof(buffer), file)) {
        TreeNode *new_tree_node;
        if (buffer[0] == 'V' || buffer[0] == 'H') {
            new_tree_node = assembleNode(&stack_head, buffer);
        } else {
            new_tree_node = createNode(buffer);
        }
        pushToStack(&stack_head, new_tree_node);
    }

    fclose(file);
    return stack_head;
}

void setCoordinates(TreeNode *tree_node) {
    if (tree_node->left_child == NULL && tree_node->right_child == NULL) {
        return;
    }

    tree_node->left_child->y_coord = tree_node->y_coord + 
                                     ((tree_node->label == 'H' && !tree_node->id) ? tree_node->right_child->height : 0);
    tree_node->right_child->x_coord = tree_node->x_coord + 
                                      ((tree_node->label == 'V' && !tree_node->id) ? tree_node->left_child->width : 0);

    tree_node->left_child->x_coord = tree_node->x_coord;
    tree_node->right_child->y_coord = tree_node->y_coord;

    setCoordinates(tree_node->left_child);
    setCoordinates(tree_node->right_child);
}

void displayTree(TreeNode *tree_node, FILE *output_pr, FILE *output_dim, FILE *output_pck) {
    if (tree_node == NULL) return;

    if (tree_node->left_child == NULL && tree_node->right_child == NULL) {
        fprintf(output_pr, "%d(%d,%d)\n", tree_node->id, tree_node->width, tree_node->height);
    } else {
        fprintf(output_pr, "%c\n", tree_node->label);
    }

    displayTree(tree_node->left_child, output_pr, output_dim, output_pck);
    displayTree(tree_node->right_child, output_pr, output_dim, output_pck);

    if ((tree_node->label == 'V' || tree_node->label == 'H') && tree_node->id == 0) {
        fprintf(output_dim, "%c(%d,%d)\n", tree_node->label, tree_node->width, tree_node->height);
    } else {
        fprintf(output_dim, "%d(%d,%d)\n", tree_node->id, tree_node->width, tree_node->height);
        fprintf(output_pck, "%d((%d,%d)(%d,%d))\n", tree_node->id, tree_node->width, tree_node->height,
                tree_node->x_coord, tree_node->y_coord);
    }
}