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
#include "exp_stack.h"
#include "scanner.h"
#include "str.h"

/**
 * @enum Rules
 */
typedef enum {

    E_LT_E,         // E < E
    E_GT_E,         // E > E
    E_GEQ_E,        // E >= E
    E_LEQ_E,        // E <= E
    E_EQ_E,         // E == E
    E_NE_E,         // E ~= E
    E_MINUS_E,      // E - E
    E_PLUS_E,       // E + E
    E_MUL_E,        // E * E
    E_DIV_E,        // E / E
    E_IDIV_E,       // E // E
    E_LEN,          // # E
    E_CONCAT_E,     // E .. E

    ID_RULE,        // id
    BR_E_BR,        // (E)
    NO_RULE
} rules_enum;

/**
 * @brief Initialization of generated code
 * @return True in case of success, false otherwise
 */
bool code_generator_init();

/**
 * @brief Write out whole generated code and free allocated string
 * @param out_file where to write out code (stdout)
 */
void code_write_out(FILE *out_file);

/**
 * @brief Free string with generated code(Call in case of error)
 */
void free_generated_code_string();

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
 * @param f_name name of called function
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
 * @param token create parameter from token
 * @param param_num index of passed parameter
 * @return true in case of success, false otherwise
 */
bool code_generate_function_parameter(token_struct token, int param_num);

/**
 * @brief Save expression result(on GF@%gl_res) on retval(LF@%retval_<retval_index>)
 * Used in user functions with >1 return values
 * @param retval_index retval index
 * @return true in case of success, false otherwise
 */
bool code_generate_save_expression_result_on_retval(int retval_index);

/**
 * @brief Generation of jump to function and
 * @param f_name name of return function
 * @return true in case of success, false otherwise
 */
bool code_generate_function_return(char *f_name);

/**
 * @brief Generation of header of function
 * @param f_name name of function
 * @return true in case of success, false otherwise
 */
bool code_generate_function_start(char *f_name);

/**
 * @brief Generation of ending of function
 * @param f_name name of function
 * @return true in case of success, false otherwise
 */
bool code_generate_function_end(char *f_name);

/**
 * @brief Generation of defenition of single retval(DEFVAR retval)
 * @return true in case of success, false otherwise
 */
bool code_generate_retval_create(int retval_index);

/**
 * @brief Push retval for needed index(retval_index) on stack: PUSHS LF@%retval_1(_2,_3,...)
 * @param retval_index retval index
 * @return true in case of success, false otherwise
 */
bool code_generate_retval_on_stack(int retval_index);

/**
 * @brief Generation of call of write function
 * @param term_count count of passed parameters
 * @return true in case of success, false otherwise
 */
bool code_generate_write_function(int terms_count, ...);

/**
 * @brief Generation of creation of variable
 * @param var_name name of created variable
 * @return true in case of success, false otherwise
 */
bool code_generate_variable_create(char *var_name);

/**
 * @brief Generation of moving token value on variable
 * @param var_name name of defined variable
 * @param token value of variable to give
 * @return true in case of success, false otherwise
 */
bool code_generate_variable_define_value(char *var_name, token_struct token);

/**
 * @brief Generation of pushing token value on the top of the stack
 * @param token token to push on top of the stack
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
 * @param r rule to execute on stack
 * @return true in case of success, false otherwise
 */
bool code_generate_operations(rules_enum r);

/**
 * @brief Generation of popping top of the stack as result
 * @return true in case of success, false otherwise
 */
bool code_generate_pop_stack_result();

/**
 * @brief Generation of IF header
 * @param if_index index of given IF (stored in code_struct->if_label_counter)
 * @return true in case of success, false otherwise
 */
bool code_generate_if_start(int if_index);

/**
 * @brief Generation of ELSE part
 * @param if_index index of given IF (stored in code_struct->if_label_counter)
 * @return true in case of success, false otherwise
 */
bool code_generate_else(int if_index);

/**
 * @brief Generation of ELSE ending (ending of IF, if ELSE was used)
 * @param if_index index of given IF (stored in code_struct->if_label_counter)
 * @return true in case of success, false otherwise
 */
bool code_generate_if_end(int if_index);

/**
 * @brief Generation of WHILE header
 * @param if_index index of given WHILE (stored in code_struct->while_label_counter)
 * @return true in case of success, false otherwise
 */
bool code_generate_while_start(int while_index);

/**
 * @brief Generation of WHILE ending
 * @param if_index index of given WHILE (stored in code_struct->while_label_counter)
 * @return true in case of success, false otherwise
 */
bool code_generate_while_end(int while_index);

#endif
