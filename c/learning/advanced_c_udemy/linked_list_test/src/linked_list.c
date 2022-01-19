#include <stdio.h>

#include "../include/linked_list.h"

void display(const struct Node* node) {
    printf("The elements are:\n");

    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }

    printf("\n");
}