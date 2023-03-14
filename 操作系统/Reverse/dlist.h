#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char*        data;
    struct Node *prev, *next;
} node_t;

typedef struct LinkedList {
    node_t* head;
    // size_t  size;
} list_t;

void* allocate(size_t n) {
    void* ptr = malloc(n);
    if (ptr == NULL) {
        fprintf(stderr, "malloc failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
void init_list(list_t* l) { l->head = NULL; }

void buy_node(node_t** node, ...) {
    *node = (node_t*)allocate(sizeof(node_t));
    va_list args;
    va_start(args, node);
    char* data    = va_arg(args, char*);
    (*node)->data = data;
    node_t* ptr   = va_arg(args, node_t*);
    (*node)->prev = ptr;
    ptr           = va_arg(args, node_t*);
    (*node)->next = ptr;
    va_end(args);
}

void destroy_list(list_t* l) {
    for (node_t* tmp = l->head; tmp; tmp = l->head) {
        l->head = tmp->next;
        free(tmp);
    }
}

void push_front(list_t* l, char* data) {
    node_t* node;
    buy_node(&node, data, NULL, l->head);
    if (l->head)
        l->head->prev = node;
    l->head = node;
}