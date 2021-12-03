/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Stack header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _EXP_STACK_H_
#define _EXP_STACK_H_

#include <stdbool.h>

#include "symtable.h"
#include "expr_handle.h"

/**
 * @enum Available symbols
 */
typedef enum {

    LT,         // <
    GT,         // >
    GEQ,        // >=
    LEQ,        // <=
    EQ,         // ==
    NE,         // ~=
    MINUS,      // -
    PLUS,       // +
    MUL,        // *
    DIV,        // /
    IDIV,       // //
    LEN,        // #
    CONCAT,     // ..

    LBR,        // (
    RBR,        // )
    ID,         // identifier
    INT,
    DOUBLE,
    STRING,

    SIGN,       // $
    STOP,
    EXPR
} elem_enum;

/**
 * @struct Stack item structure
 */
typedef struct item_stack_struct {

    tab_item_data_type type;
    elem_enum elem;
    struct item_stack_struct *nxt;
} item_stack_t;

/**
 * @struct Stack structure
 */
typedef struct stack_struct {

    item_stack_t *top;
} stack_t;

/**
 * @brief Stack initialization
 * @param stack Pointer on stack
 */
void init_stack(stack_t *stack);

/**
 * @brief Check if stack is empty
 * @param stack Pointer on stack
 */
bool is_empty_stack(stack_t *stack);

/**
 * @brief Found and return item on top of the stack
 * @param stack Pointer on stack
 * @return Pointer on stack top item
 */
item_stack_t *stack_top(stack_t *stack);

/**
 * @brief Found and return terminal until stop symobol
 * @param stack Pointer on stack
 * @return Pointer on stack terminal top item
 */
item_stack_t *stack_top_term(stack_t *stack);

/**
 * @brief Pop top item from the stack
 * @param stack Pointer on stack
 * @return true in case of success, false in case of empty stack
 */
bool pop_stack(stack_t *stack);

/**
 * @brief Push new item on top of the stack
 * @param stack Pointer on stack
 * @param elem New character
 * @param type Type of new element
 * @return true in case of success, else false
 */
bool push_stack(stack_t *stack, elem_enum elem, tab_item_data_type type);

/**
 * @brief Clear entire stack from the items
 * @param stack Pointer on stack
 */
void empty_stack(stack_t *stack);

/**
* @brief Insert new item after top terminal in stack
* @param stack Pointer on stack
* @param elem New character
* @param type Type of new element
* @return true in case of success, else false
 */
bool insert_after_top_term(stack_t *stack, elem_enum elem, tab_item_data_type type);

#endif
