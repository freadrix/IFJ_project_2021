/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief data stack header
 * @author Anton Medvedev (xmedve04)
 * */

#ifndef _DATA_STACK_H_
#define _DATA_STACK_H_

#include "symtable.h"
#include <stdbool.h>


// Structs
/**
 * @struct data stack item
 * */
typedef struct item_of_stack{
    table_t *table;
    struct item_of_stack *next;
    struct item_of_stack *previous;
}item_data_stack_t;

/**
 * @struct data stack struct
 * */
typedef struct {
    item_data_stack_t *top;
}data_stack_t;


// Functions
/**
 * @param stack pointer on stack
 * @brief func initialize stack
 * */
void init_data_stack(data_stack_t *stack);

/**
 * @param stack pointer on stack
 * @brief func check if stack is empty
 * @return result of check
 * */
bool data_stack_is_empty(data_stack_t *stack);

/**
 * @param stack pointer on stack
 * @brief func return first frame of stack
 * @return first element of stack
 * */
item_data_stack_t *get_global_frame_stack(data_stack_t *stack);

/**
 * @param stack pointer on stack
 * @brief func alloc memory and add one more element to top of stack
 * @return result of adding true if success else false
 * */
bool push_data_item(data_stack_t *stack);

/**
 * @param stack pointer on stack
 * @brief func free memory of top element of stack and remove him
 * @return result of pop
 * */
bool pop_data_item(data_stack_t *stack);

/**
 * @param stack pointer on stack
 * @brief funk delete all stack
 * */
void empty_data_stack(data_stack_t *stack);

#endif
