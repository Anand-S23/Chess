#ifndef STACK_H
#define STACK_H

typedef struct node
{
    struct node* next;
    move data;
} node;

typedef struct linked_list
{
    node* head;
} linked_list;

node* CreateNode(move data, node *next);
linked_list CreateList(); 
void Push(linked_list list, move data);
move Pop(linked_list list);
move Top(linked_list list);
int Empty(linked_list list);

#endif