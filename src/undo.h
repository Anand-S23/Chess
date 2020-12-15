#ifndef STACK_H
#define STACK_H

typedef struct move
{
    piece_type current_piece; 
    int current_i; 
    int current_j; 
    piece_type original_piece; 
    int original_i; 
    int original_j; 
} move;

typedef struct node
{
    struct node* next;
    move data;
} node;

typedef struct stack
{
    node *head;
} stack;

internal node* CreateNode(move data, node *next);
internal stack CreateStack(); 
internal void Push(stack list, move data);
internal move Pop(stack list);
internal int Empty(stack list);

#endif