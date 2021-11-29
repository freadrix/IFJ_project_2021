/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Syntactic and semantic check
 * @author Matej Alexej Helc
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
//    stack_t
    //TODO

    int if_label_counter;       //counter of IF's used in code (for labels)
    int while_label_counter;    //counter of WHILE's used in code (for labels)
} code_struct;


// functions declaration
int get_token_check() ;
int p_params();
int p_rets(table_item*);        //ERROR: error here, was renamed
int p_body();
int p_defvar();
int p_assign();
int p_state_l();
int p_while();
int p_if();
int p_params_in();

#endif
