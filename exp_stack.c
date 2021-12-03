/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Stack
 * @author Aleksandr Verevkin (xverev00)
 */

#include <stdio.h>
#include <stdlib.h>

#include "exp_stack.h"


void init_stack(stack_t *stack) {
    //initialization of stack
    stack->top = NULL;
}

bool is_empty_stack(stack_t *stack) {
    //check if stack is empty
    if (stack->top == NULL) {
        return true;
    } else {
        return false;
    }
}

item_stack_t *stack_top(stack_t *stack) {
    return stack->top;
}

item_stack_t *stack_top_term(stack_t *stack) {
    item_stack_t *iter_item = stack_top(stack);
    while (iter_item) {
        if (iter_item->elem < STOP) {
            return iter_item;
        }
        iter_item = iter_item->nxt;
    }
    return NULL;
}

bool pop_stack(stack_t *stack) {
    //pop top item from the stack
    if (!(stack->top)) {
        return false;
    } else {
        item_stack_t *item = stack->top;
        stack->top = item->nxt;
        free (item);
        return true;
    }
}

bool push_stack(stack_t *stack, elem_enum elem, tab_item_data_type type) {
    //push item on the top of stack
    item_stack_t *item = malloc(sizeof(item_stack_t));
    if (!(item)) {
        return false;
    } else {
        item->nxt = stack->top;
        item->elem = elem;
        item->type = type;
        stack->top = item;
        return true;
    }
}

void empty_stack(stack_t *stack) {
    //clear stack from the elements
    bool tmp = true;
    while (tmp) {
        tmp = pop_stack(stack);
    }
}

bool insert_after_top_term(stack_t *stack, elem_enum elem, tab_item_data_type type) {
    item_stack_t *tmp = stack_top(stack);
    item_stack_t *previous = NULL;

    while (tmp != NULL) {
        if (tmp->elem < STOP) {
            item_stack_t *new = malloc(sizeof(item_stack_t));
            if (!new) {
                return false;
            } else {
                new->elem = elem;
                new->type = type;

                if (!previous) {
                    new->nxt = stack->top;
                    stack->top = new;
                } else {
                    new->nxt = previous->nxt;
                    previous->nxt = new;
                }

                return true;
            }
        }
        previous = tmp;
        tmp = tmp->nxt;
    }
    return false;
}
