#include <stdio.h>
#include <stdlib.h>

/**
 * Structure for a doubly linked list, containing integer data.
 */
struct Node_int
{
    int data;
    struct Node_int* prev;
    struct Node_int* next;
};

void print_list(struct Node_int* root)
{
    if (root == NULL)
    {
        printf("List is empty\n");
        return;
    }

    struct Node_int* last;

    printf("Traversing in forward direction:\n");
    while (root != NULL)
    {
        printf(" %d, ", root->data);
        last = root;
        root = root->next;
    }

    printf("\nTraversing in backward direction:\n");
    while (last != NULL) {
        printf(" %d, ", last->data);
        last = last->prev;
    }

    printf("\n");
}

struct Node_int* make_node(const int value)
{
    struct Node_int* node = malloc(sizeof(struct Node_int));
    node->data = value;

    return node;
}

void clear_list(struct Node_int** root)
{
    struct Node_int* head = *root;

    while (head != NULL)
    {
        struct Node_int* node = head->next;

        free(head);

        head = node;
    }

    *root = NULL;
}

void push_front(struct Node_int** root, const int value)
{
    struct Node_int* node = make_node(value);

    /* Set the next pointer of the first node link to head and make
       the prev pointer link to NULL, as it is to become the head.
    */
    node->next = *root;
    node->prev = NULL;

    if (*root != NULL)
    {
        (*root)->prev = node;
    }

    *root = node;
}

void push_back(struct Node_int** root, const int value)
{
    struct Node_int* node = make_node(value);

    node->next = NULL;

    if (*root == NULL) // New node becomes the root if the root is NULL.
    {
        node->prev = NULL;
        *root = node;
    }
    else
    {
        struct Node_int *last = *root;

        while (last->next != NULL)
        {
            last = last->next;
        }

        // Last node points now to the newly created node.
        last->next = node;

        // Don't forget to point to the previous node.
        node->prev = last;
    }
}

void push_after(struct Node_int* prev_node, const int value)
{
    if (prev_node == NULL)
    {
        printf("Cannot add after a NULL node\n");
        return;
    }

    struct Node_int* node = make_node(value);

    // Make the new node point to the next of the current one.
    node->next = prev_node->next;

    // Update the next pointer of the current node.
    prev_node->next = node;

    // Make the new node prev pointer point to the current one.
    node->prev = prev_node;

    // Update the prev pointer of the next node if that one is not NULL.
    if (node->next != NULL)
    {
        node->next->prev = node;
    }
}

void test_dll(void)
{
    struct Node_int* root = NULL;

    print_list(root);

    push_back(&root, 4);

    push_after(root->next, 6); // should not add
    push_after(root, 6); // should add

    push_front(&root, 3);
    push_front(&root, 7);
    push_back(&root, 8);
    push_after(root->next->next, 2);
    push_front(&root, 9);

    print_list(root);

    clear_list(&root);

    print_list(root);
}

int main(void)
{
    test_dll();

    return 0;
}