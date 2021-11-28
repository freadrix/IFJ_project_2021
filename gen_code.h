/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Code generation header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _GEN_CODE_H_
#define _GEN_CODE_H_

#include <stdbool.h>
#include <stdarg.h>

#include "symtable.h"
#include "expr_handle.h"
#include "scanner.h"
#include "str.h"


/**
 * @brief Initialization of generated code
 * @return True in case of success, false otherwise
 */
bool code_generator_init();

/**
 * @brief Write out whole generated code and free allocated string
 */
void code_write_out(FILE *out_file);

/**
 * @brief Generation of header of main scope
 * @return true in case of success, false otherwise
 */
bool code_generate_main_start();

/**
 * @brief Generation of ending of main scope
 * @return true in case of success, false otherwise
 */
bool code_generate_main_end();

/**
 * @brief Generation of function call
 * @return true in case of success, false otherwise
 */
bool code_generate_function_call(char *f_name);

/**
 * @brief Generation of command that clear frame variables
 * @return true in case of success, false otherwise
 */
bool code_generate_empty_variables_frame();

/**
 * @brief Generation of parameters to pass into functions
 * @return true in case of success, false otherwise
 */
bool code_generate_function_parameter(token_struct token, int param_num);

/**
 * @brief Generation of jump to function and
 * @return true in case of success, false otherwise
 */
bool code_generate_function_return(char *f_name);

/**
 * @brief Generation of header of function
 * @return true in case of success, false otherwise
 */
bool code_generate_function_start(char *f_name);

/**
 * @brief Generation of ending of function
 * @return true in case of success, false otherwise
 */
bool code_generate_function_end(char *f_name);

/**
 * @brief Generation of call of write function
 * @return true in case of success, false otherwise
 */
bool code_generate_write_function(int terms_count, ...);

/**
 * @brief Generation of length operation
 * @return true in case of success, false otherwise
 */
bool code_generate_length();

/**
 * @brief Generation of concatenation operation
 * @return true in case of success, false otherwise
 */
bool code_generate_concatenation();

/**
 * @brief Generation of creation of variable
 * @return true in case of success, false otherwise
 */
bool code_generate_variable_create(char *var_name);

/**
 * @brief Generation of defining variable type
 * @return true in case of success, false otherwise
 */
bool code_generate_variable_define_type(char *var_name, tab_item_data_type type);

/**
 * @brief Generation of moving token value on variable
 * @return true in case of success, false otherwise
 */
bool code_generate_variable_define_string(char *var_name, token_struct token);

/**
 * @brief Generation of pushing token value on the top of the stack
 * @return true in case of success, false otherwise
 */
bool code_generate_stack_push(token_struct token);

/**
 * @brief Generation of convertion of first stack variable from integer to float
 * @return true in case of success, false otherwise
 */
bool code_generate_stack_convert_float_first();

/**
 * @brief Generation of convertion of second stack variable from integer to float
 * @return true in case of success, false otherwise
 */
bool code_generate_stack_convert_float_second();

/**
 * @brief Generation of convertion of first stack variable from float to integer
 * @return true in case of success, false otherwise
 */
bool code_generate_stack_convert_int_first();

/**
 * @brief Generation of convertion of second stack variable from float to integer
 * @return true in case of success, false otherwise
 */
bool code_generate_stack_convert_int_second();

/**
 * @brief Generation of rules operations in the stack
 * @return true in case of success, false otherwise
 */
bool code_generate_operations(rules_enum r);

/**
 * @brief Generation of popping top of the stack as result
 * @return true in case of success, false otherwise
 */
bool code_generate_pop_stack_result();


#endif
