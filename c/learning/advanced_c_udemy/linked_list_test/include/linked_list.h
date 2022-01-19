#ifndef LINKED_LIST_INCLUDED
#define LINKED_LIST_INCLUDED

struct Node
{
    int data;
    struct Node *next;
};

void display(const struct Node* node);


#endif // LINKED_LIST_INCLUDED