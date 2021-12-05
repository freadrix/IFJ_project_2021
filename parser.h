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
//#include "exp_stack.h"
#include "data_stack.h"
#include "error.h"
#include "gen_code.h"


// Functions declaration
// TODO move functions in right order
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
 * @brief check syntax and semantics in function definition
 * @return return code which is in error.h
 * */
int function_parser();

/**
 * @brief check syntax and semantics of function parameters
 *        in function definition
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int function_params_parser(tab_item_t *function_item);

/**
 * @brief check syntax and semantics of function return values
 *        in function definition
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int function_rets_parser(tab_item_t *function_item);

/**
 * @brief check syntax and semantics of function body values
 *        in function definition
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int function_body_parser(tab_item_t *function_item);

/**
 * @brief check syntax and semantics of return command
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int return_parser(tab_item_t *function_item);

/**
 * @brief check syntax and semantics of while loop
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int while_parser(tab_item_t *function_item);

/**
 * @brief check syntax and semantics of if else condition
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int if_parser(tab_item_t *function_item);

/**
 * @brief check syntax and semantics of variable definition (declaration)
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int def_var_parser(tab_item_t *function_item);

/**
 * @brief function TODO
 * @param function_item element of table with function struct
 * @return return code which is in error.h
 * */
int id_in_body_parser(tab_item_t *function_item);

/**
 * @brief check syntax and semantics in function declaration
 * @return return code which is in error.h
 * */
int global_parser();

/**
 * @brief check syntax and semantics in function declaration
 * @param item element of table with function struct
 * @return return code which is in error.h
 * */
int global_function_parser(tab_item_t *item);

/**
 * @brief check if there is '(' after function ID
 * @return return code which is in error.h
 * */
int call_check_parser();

/**
 * @brief check syntax and semantics of (called) function parameters
 * @return return code which is in error.h
 * */
int call_function_parser();

#endif
