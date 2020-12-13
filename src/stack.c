#include "stack.h"

// makes a new node using malloc
node* CreateNode(move data, node* next)
{
    node* ret_node = (node *)malloc(sizeof(node));
    ret_node->data = data;
    ret_node->next = next;
    return ret_node;
}

// makes a new list with head of {0, NULL}
linked_list CreateList()
{
    linked_list ret_list;
    move new_move = {0};
    ret_list.head = CreateNode(new_move, NULL);
    return ret_list; 
} 

void Push(linked_list list, move data)
{
    node* in_node = CreateNode(data, NULL);

    in_node->next = list.head->next;
    list.head->next = in_node;
}

move Pop(linked_list list)
{
    move data = list.head->next->data;
    node *temp = list.head->next;
    list.head->next = list.head->next->next;
    free(temp);
    return data;
}
    
move Top(linked_list list)
{
    return list.head->next->data;
}

int Empty(linked_list list)
{
    return list.head->next == NULL;
}