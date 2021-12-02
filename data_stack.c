/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief data stack
 * @author Anton Medvedev (xmedve04)
 * */

#include <stdio.h>
#include <stdlib.h>
#include "data_stack.h"


void init_data_stack(data_stack_t *stack) {
    stack->top = NULL;
}

bool data_stack_is_empty(data_stack_t *stack) {
    return (stack->top == NULL ? true : false);
}

item_data_stack_t *get_global_frame_stack(data_stack_t *stack) {
    item_data_stack_t *element = stack->top;
    while (element->previous != NULL){
        element = element->previous;
    }
    return element;
}

bool push_data_item(data_stack_t *stack) {
    item_data_stack_t *item = malloc(sizeof(item_data_stack_t));
    if (!(item)) return false;
    item->previous = stack->top;
    if (!data_stack_is_empty(stack)) stack->top->next = item;
    item->next = NULL;
    stack->top = item;
    item->table = malloc(MAX_HT_SIZE * sizeof(tab_item_t*));
    init_hashtable(item->table);
    return true;
}

bool pop_data_item(data_stack_t *stack) {
    if (stack->top == NULL) return false;
    item_data_stack_t *item = stack->top;
    delete_all_hashtable(item->table);
    free(item->table);
    stack->top = item->previous;
    if(!data_stack_is_empty(stack)) {
        stack->top->next = NULL;
    }
    free(item);
    return true;
}

void empty_data_stack(data_stack_t *stack) {
    bool tmp = true;
    while (tmp) {
        tmp = pop_data_item(stack);
    }
}
