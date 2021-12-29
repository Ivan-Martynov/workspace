#ifndef _SINGLELL_H
#define _SINGLELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct node {
    uint8_t data;
    struct node *next;
} node_t;

uint8_t list_length(node_t*);
void print_list(node_t*);
void add_first(node_t**, uint8_t);
void add_last(node_t**, uint8_t);
void add_element(node_t**, uint8_t, uint8_t);
int remove_first(node_t**);
int remove_last(node_t**);
int8_t remove_by_index(node_t**, uint8_t);
int remove_by_value(node_t**, uint8_t);

#endif
