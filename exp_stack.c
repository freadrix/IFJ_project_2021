/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Stack
 * @author Aleksandr Verevkin (xverev00)
 */

#include <stdio.h>
#include <stdlib.h>

#include <exp_stack.h>

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

bool push_stack(stack_t *stack, char elem) {
    //push item on the top of stack
    item_stack_t *item = malloc(sizeof(item_stack_t));
    if (!(item)) {
        return false;
    } else {
        item->nxt = stack_top;
        item->elem = elem;
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