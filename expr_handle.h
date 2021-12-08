/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Expression handling header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _EXPR_HANDLE_H_
#define _EXPR_HANDLE_H_

#include "exp_stack.h"
#include "symtable.h"
#include "data_stack.h"
#include "scanner.h"
#include "error.h"
#include "gen_code.h"

/**
 * @brief Main expression processing function
 * @param token Start token of the expression
 * @param data_stack Pointer on stack with variables
 * @param expression_type Final expression type
 * @return OK(0) in case of correctly writed expression, else one of the errors
 */
int exp_processing(token_struct *token, data_stack_t *data_stack, tab_item_data_type *expression_type);

#endif
