#include <stdio.h>
#include <stdlib.h>

/**
 * Structure for a doubly linked list, containing integer data.
 */
struct Node_int
{
    int data;
    struct Node_int* next;
};

void print_list(struct Node_int* root)
{
    if (root == NULL)
    {
        printf("List is empty\n");
        return;
    }
    else
    {
        printf("Traversing the list:\n");

        while (root != NULL)
        {
            printf(" %d, ", root->data);
            root = root->next;
        }

        printf("\n");
    }
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

    *root = node;
}

void push_back(struct Node_int** root, const int value)
{
    struct Node_int* node = make_node(value);

    node->next = NULL;

    if (*root == NULL) // New node becomes the root if the root is NULL.
    {
        *root = node;
    }
    else
    {
        struct Node_int* last = *root;

        while (last->next != NULL)
        {
            last = last->next;
        }

        // Last node points now to the newly created node.
        last->next = node;
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
}

void test_sll(void)
{
    struct Node_int* root = NULL;

    print_list(root);

    push_after(root, 6);

    push_back(&root, 4);
    push_front(&root, 3);
    push_back(&root, 7);
    push_after(root->next, 6);
    push_back(&root, 9);

    print_list(root);

    clear_list(&root);

    print_list(root);
}

int main(void)
{
    test_sll();

    return 0;
}
