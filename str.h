/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief String operations header
 * @author Ivan Tsiareshkin (xtsiar00)
 */

#ifndef _STR_H_
#define _STR_H_

#include <stdlib.h>
#include <string.h>

#define STRING_LEN_INC 8

#define STR_ERR 0
#define STR_OK 1

typedef struct {
    
	char *string;
    size_t length;
    size_t alloc_length;
} string_struct;

int string_init(string_struct *str);

void string_free(string_struct *str);

void string_clear(string_struct *str);

int add_char_to_string(string_struct *str, char c);

/**
 * @brief Add string to a string structure, primary used for code generation
 * @param str Pointer on structure
 * @param str_to_add String that will be added
 * @return STR_OK(1) in case of success, STR_ERR(0) else
 */
int add_string_to_string(string_struct *str, const char *str_to_add);

#endif
