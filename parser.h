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
#include "str.h"
#include "symtable.h"
#include "exp_stack.h"
#include "data_stack.h"
#include "error.h"


//typedef struct {
//
//    token_struct tok;
////    table_t local; //  think that wont need
//    table_t global;
////    stack_t
//    //TODO
//} code_struct;


// Functions declaration
/**
 * @brief cycle which will get token and work out him
 * @return return code which is in error.h
 * */
int parser();

/**
 * @brief check if in program exist require keyword and "ifj21" next him
 *        after that parser will start work
 * @return return code which is in error.h
 * */
int start_program_parser();

/**
 * @brief function TODO
 * @return return code which is in error.h
 * */
int function_parser();

/**
 * @brief function TODO
 * @return return code which is in error.h
 * */
int global_parser();

/**
 * @brief TODO
 * @return return code which is in error.h
 * */
int global_function_parser();


//int p_params();
//int p_rets(table_item*);
//int p_body();
//int p_defvar();
//int p_assign();
//int p_state_l();
//int p_while();
//int p_if();
//int p_params_in();

#endif
