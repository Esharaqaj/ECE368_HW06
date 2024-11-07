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
    if (*stack_head == NULL || (*stack_head)->next == NULL) {
        return NULL;
    }

    StackNode *temp = (*stack_head)->next;
    TreeNode *removed_node = temp->tree;
    (*stack_head)->next = temp->next;
    free(temp);

    return removed_node;
}

void pushToStack(StackNode **stack_head, TreeNode *new_tree_node) {
    if (new_tree_node == NULL) {
        fprintf(stderr, "Error: Attempting to push a NULL tree node onto the stack.\n");
        return;
    }

    StackNode *new_stack_node = malloc(sizeof(StackNode));
    if (new_stack_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE); // Exit if malloc fails to avoid undefined behavior.
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
    while (stack_head != NULL) {
        StackNode *next_node = stack_head->next;
        if (stack_head->tree != NULL) {
            freeTree(stack_head->tree);
        }
        free(stack_head);
        stack_head = next_node;
    }
}

// Added checks in createTree function
StackNode *createTree(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s.\n", filename);
        return NULL;
    }

    StackNode *stack_head = malloc(sizeof(StackNode));
    if (stack_head == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }
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
        if (new_tree_node != NULL) {
            pushToStack(&stack_head, new_tree_node);
        }
    }

    fclose(file);
    return stack_head;
}