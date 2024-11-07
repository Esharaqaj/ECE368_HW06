
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <math.h>


typedef struct Node
{ 
    char node_name; // print out the ASCII representation of the numbers and characters
    int int_name;
    int width; 
    int height;
    int org_x;
    int org_y;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Stack
{ 
    struct Node * node;
    struct Stack * next;
} Stack;

struct Stack * buildTree(char * filename);
void free_stack(Stack * stack_head);
void print_node(Node * root, FILE * fol_pr, FILE * fol_dim, FILE * fol_pck);
void  box_and_room_coords(Node * root);
struct Stack * buildTree(char * filename);
struct Node * build_trio(Stack ** stack_head, char * buffer);
void free_tree(Node * root);
void add_to_stack(Stack ** stack_head, Node * new_node);
struct Node * init_node(char * buffer);
struct Node * pop_from_stack(Stack ** stack_head);
