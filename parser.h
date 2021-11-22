/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Syntactic and semantic check
 * @author TODO
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"
#include "exp_stack.h"


typedef struct {

    token_struct tok;
//    table_t local; //  think that wont need
    table_t global;
    stack_t
    //TODO
} code_struct;


// functions declaration
void init_code_struct(code_struct *data);

#endif
