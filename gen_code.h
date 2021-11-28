/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Code generation header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _GEN_CODE_H_
#define _GEN_CODE_H_

#include <stdbool.h>

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
 * @brief Generation of header of function
 * @return true in case of success, false otherwise
 */
bool code_generate_function_start(char *f_name);

/**
 * @brief Generation of ending of function
 * @return true in case of success, false otherwise
 */
bool code_generate_function_end(char *f_name);


#endif
