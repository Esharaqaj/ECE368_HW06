
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "a6.h"


struct Node * pop_from_stack(Stack ** stack_head) {

    if (*stack_head == NULL) {

        return NULL;

    }

    Stack * temp = (*stack_head)->next; 

    Node * popped_node = temp->node;

    (*stack_head)->next = temp->next;

    free(temp);

    return popped_node;

}



struct Node * init_node(char * buffer) {

                // creating the node itself

    int cn = 0;

    int cx = 0;

    int cy = 0;

    sscanf(buffer, "%d(%d,%d)", &cn, &cx, &cy);

    struct Node * new_node = malloc(sizeof(Node));

    new_node->int_name = cn;

    new_node->node_name  = '\0';

    new_node->width = cx;

    new_node->height = cy;

    new_node->org_x = 0;

    new_node->org_y = 0;

    new_node->left = NULL;

    new_node->right = NULL;

    return new_node;

}



void add_to_stack(Stack ** stack_head, Node * new_node) {

    struct Stack * new_stack = malloc(sizeof(Stack));

    if (new_stack == NULL) {

        return;

    }

    new_stack->node = new_node;

    new_stack->next = (*stack_head)->next;

    (*stack_head)->next = new_stack;

    return;

}



void free_tree(Node * root) {

    if (root == NULL) return;

    free_tree(root->left);

    free_tree(root->right);

    free(root);

}



void free_stack(Stack * stack_head) {

    Stack * current = stack_head->next;

    while (current != NULL) {

        Stack *next_free = current->next;

        if (current->node != NULL) {

            free_tree(current->node);

        }

        free(current);

        current = next_free;

    }

}



struct Node * build_trio(Stack ** stack_head, char * buffer) {

    char l_h;

    sscanf(buffer, "%c", &l_h);

 

    struct Node * int_node = malloc(sizeof(Node));

    int_node->node_name = l_h;

    int_node->int_name = 0;

    int_node->right = pop_from_stack(stack_head);

    int_node->left = pop_from_stack(stack_head);



    if (int_node->node_name == 'V') {

        int_node->height = (int_node->left->height > int_node->right->height ? int_node->left->height : int_node->right->height);

        int_node->width = (int_node->left->width + int_node->right->width);

    } else if (int_node->node_name == 'H') {

        int_node->width = (int_node->left->width > int_node->right->width ? int_node->left->width : int_node->right->width );

        int_node->height = (int_node->left->height + int_node->right->height);

    } else {

        int_node->width = 0;

        int_node->height = 0;

    }



    int_node->org_x = 0;

    int_node->org_y = 0;

    return int_node;

}



struct Stack * buildTree(char * filename) {



    FILE * fol;

    fol = fopen(filename, "r");

    if (fol == NULL){

        return NULL;

    }



    struct Stack * stack_head = malloc(sizeof(Stack));

    stack_head->node = NULL;

    stack_head->next = NULL;

    char buffer[20];

    while (fgets(buffer, sizeof(buffer), fol))

    {   

        Node * new_node;

        if(buffer[0] == 'V' || buffer[0] == 'H') {

        new_node = build_trio(&stack_head, buffer);

        }   

        else {

        new_node = init_node(buffer); 

        }

        add_to_stack(&stack_head, new_node);

    }



    fclose(fol);

    return stack_head;

}



void box_and_room_coords(Node * tree_node) {

    if(tree_node->left == NULL && tree_node->right == NULL) {

        return;

    }



    tree_node->left->org_y = tree_node->org_y + ((tree_node->node_name == 'H' && !tree_node->int_name) ? tree_node->right->height : 0);

    tree_node->right->org_x = tree_node->org_x + ((tree_node->node_name == 'V' && !tree_node->int_name) ? tree_node->left->width : 0);

    tree_node->left->org_x = tree_node->org_x;

    tree_node->right->org_y = tree_node->org_y;

    

    box_and_room_coords(tree_node->left);

    box_and_room_coords(tree_node->right);



    return;

}



void print_node(Node * tree_node, FILE * fol_pr, FILE * fol_dim, FILE * fol_pck) {

    

    if (tree_node == NULL) return;    

    

    if (tree_node->left == NULL && tree_node->right == NULL) {

        fprintf(fol_pr, "%d(%d,%d)\n", tree_node->int_name, tree_node->width, tree_node->height);

    } else {

        fprintf(fol_pr, "%c\n", tree_node->node_name);

    }



    print_node(tree_node->left, fol_pr, fol_dim, fol_pck);

    print_node(tree_node->right, fol_pr, fol_dim, fol_pck);



    if ((tree_node->node_name == 'V' || tree_node->node_name == 'H') && tree_node->int_name == 0) {

        fprintf(fol_dim, "%c(%d,%d)\n", tree_node->node_name, tree_node->width, tree_node->height);

    } else {

        fprintf(fol_dim, "%d(%d,%d)\n", tree_node->int_name, tree_node->width, tree_node->height);

        fprintf(fol_pck, "%d((%d,%d)(%d,%d))\n", tree_node->int_name, tree_node->width, tree_node->height, tree_node->org_x, tree_node->org_y);

    }

}

