/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief String operations header
 * @author Ivan Tsiareshkin (xtsiar00)
 */

#ifndef _STR_H_
#define _STR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STRING_CONST_LENGHT 16

#define STR_ERR 0
#define STR_OK 1

typedef struct {

	char *string;
	int length;
	int alloc_length;
} string_struct;

/**
* @brief function initializes the string
* @param str Pointer on structure
* @return STR_OK(1) in case of success, otherwise STR_ERR(0)
*/
int string_init(string_struct *str);

/**
* @brief function frees memory for the string
* @param str Pointer on structure
* @return STR_OK(1) in case of success, otherwise STR_ERR(0)
*/
void string_free(string_struct *str);

/**
* @brief function clears the string
* @param str Pointer on structure
* @return STR_OK(1) in case of success, otherwise STR_ERR(0)
*/
void string_clear(string_struct *str);

/**
* @brief function re-allocates memory for the string
* @param str Pointer on structure
* @return STR_OK(1) in case of success, otherwise STR_ERR(0)
*/
int re_lenght(string_struct *str);

/**
* @brief function copies one string into another
* @param str_to_copy String to copy
* @param copy_here String, where to copy
* @return STR_OK(1) in case of success, otherwise STR_ERR(0)
*/
int string_copy(string_struct *str_to_copy, string_struct *copy_here);

/**
* @brief function inserts character in the string
* @param str Pointer on structure
* @param c Character to be inserted in the string
* @return STR_OK(1) in case of success, otherwise STR_ERR(0)
*/
int add_char_to_string(string_struct *str, int c);

/**
 * @brief function adds a string to the string structure, mostly used for code generation
 * @param str Pointer on structure
 * @param str_to_add String that will be added
 * @return STR_OK(1) in case of success, otherwise STR_ERR(0)
 */
int add_string_to_string(string_struct *str, const char *str_to_add);

#endif
