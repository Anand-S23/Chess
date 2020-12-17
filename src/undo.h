#ifndef STACK_H
#define STACK_H

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