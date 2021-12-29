#include "singleLL.h"

int main()
{
    node_t *head = 0;
    printf("List length %d\n", list_length(head));
    remove_first(&head);
    remove_last(&head);
    printf("Removed by index %d\n", remove_by_index(&head, 0));

    add_first(&head, 0);
    printf("Removed by index %d\n", remove_by_index(&head, 0));
    printf("Removed %d\n", remove_last(&head));

    add_last(&head, 3);

    printf("List length %d\n", list_length(head));

    add_element(&head, 2, 18);

    print_list(head);
    printf(" \n");

    printf("Removed %d\n", remove_first(&head));

    print_list(head);
    printf(" \n");

    add_first(&head, 5);
    add_last(&head, 13);

    printf("Removed %d\n", remove_last(&head));
    print_list(head);
    printf(" \n");


    add_first(&head, 2);
    add_element(&head, 2, 8);

    print_list(head);
    printf(" \n");

    add_first(&head, 9);
    printf("Removed by value %d\n", remove_by_value(&head, 9));
    add_last(&head, 19);
    add_first(&head, 7);

    print_list(head);
    printf("Removed by index %d\n", remove_by_index(&head, 9));
    print_list(head);
    printf(" \n");

    print_list(head);
    printf("List length %d\n", list_length(head));

    return 0;
}
