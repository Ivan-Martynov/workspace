#include "singleLL.h"

/* Function list_length
 * -----------------------------
 *  computes the length of the single linked list
 */
uint8_t list_length(node_t *head)
{
    node_t *temp = head;
    uint8_t i = 0;

    while (temp) {
        temp = temp->next;
        i++;
    }

    return i;
}

/* Function print_list
 * -----------------------------
 *  prints the content of the single linked list
 */
void print_list(node_t *head)
{
    node_t *temp = head;

    while (temp) {
        printf("temp->data: %d\n", temp->data);
        temp = temp->next;
    }
}

/* Function add_first:
 * -----------------------------
 *  adds an element to the front of the list
 */
void add_first(node_t **head, uint8_t data)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = *head;
    *head = new_node;
}

/* Function add_last:
 * -----------------------------
 *  adds an element to the end of the list
 */
void add_last(node_t **head, uint8_t data)
{
    node_t *new_node = malloc(sizeof(*new_node));
    new_node->data = data;
    new_node->next = 0;

    /* handle the case of the empty list */
    if (!*head) {
        *head = new_node;
        return;
    }

    node_t *temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
}

/* Function add_element:
 * -----------------------------
 *  adds an element to the list to the index (ind) with data (data)
 *  this function can be used for inserting elements at the
 *  beginning and the end of the list
 */
void add_element(node_t **head, uint8_t ind, uint8_t data)
{
    /* init new node with data */
    node_t *new_node = malloc(sizeof(*new_node));
    new_node->data = data;

    /* add to the front of the list */
    if ((!*head) || (ind <= 0)) {
        new_node->next = *head;
        *head = new_node;
        return;
    }

    /* loop until the desired element is found or the end
     * of the list is reached
     */
    node_t *temp = *head;
    for (uint8_t i = 0; i < ind - 1; i++) {
        if (!temp->next) break;
        temp = temp->next;
    }
    new_node->next = temp->next;
    temp->next = new_node;
}

/* Function remove_first
 * ---------------------------------
 *  removes the first element from the list
 */
int remove_first(node_t **head)
{
    if (!*head) {
        printf("The list is empty\n");
        return -1;
    }

    int r = (*head)->data;
    node_t *temp = *head;
    *head = (*head)->next;
    free(temp);

    return r;
}

/* Function remove_last
 * ---------------------------------
 *  removes the last element from the list
 */
int remove_last(node_t **head)
{
    if (!*head) {
        printf("The list is empty\n");
        return -1;
    }

    /* handle the case of one-element-list */
    if (!(*head)->next)
        return remove_first(&(*head));

    node_t *temp = *head;
    while (temp->next->next)
        temp = temp->next;
    int r = temp->next->data;
    free(temp->next);
    temp->next = 0;

    return r;
}

/* Function remove_by_index
 * -------------------------------------------
 *  removes an element by its index in the list
 */
int8_t remove_by_index(node_t **head, uint8_t ind)
{
    if (!*head) {
        printf("The list is empty\n");
        return -1;
    }

    /* handle the case of one-element-list */
    if ((!(*head)->next) || (ind <= 0))
        return remove_first(&(*head));

    node_t *prev = *head;

    for (uint8_t i = 0; i < ind - 1; i++) {
        if (!prev->next) break;
        prev = prev->next;
    }

    if (!prev->next)
        return remove_last(&(*head));

    node_t *temp = prev->next;
    int8_t r = temp->data;
    prev->next = temp->next;
    free(temp);

    return r;
}

/* Function remove_by_value
 * -------------------------------------------
 *  finds the first element in the list equal to the value and removes it
 *  if element is not found the list is not changed
 */
int remove_by_value(node_t **head, uint8_t val)
{
    if (!*head) {
        printf("The list is empty\n");
        return -1;
    }

    /* handle the case of one-element-list */
    if (val == (*head)->data) {
        return remove_first(&(*head));
    } else if (!(*head)->next) {
        printf("No element with such value in the list\n");
        return -1;
    }

    node_t *prev = *head;
    node_t *temp = *head;
    int r;

    while (prev->next) {
        r = prev->next->data;
        if (r == val) {
            temp = prev->next;
            prev->next = temp->next;
            free(temp);
            return r;
        }
        prev = prev->next;
    }

    printf("No element with such value in the list\n");
    return -1;
}
