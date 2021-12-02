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

#define STRING_CONST_LENGHT 8

#define STR_ERR 0
#define STR_OK 1

typedef struct {

	char *string;
	int length;
	int alloc_length;
} string_struct;

// Functions
///**
// * @brief functiong count number of characters
// * @param string
// * @return amount of characters in string
// * */
//int string_length(char *string);
//
///**
// * @brief function compare strings and return true if it success
// * @param string1 first string
// * @param string2 second string
// * @return true if strings the same, otherwise false
// * */
//bool string_compare(char *string1, char *string2);

int string_init(string_struct *str);

void string_free(string_struct *str);

void string_clear(string_struct *str);

int re_lenght(string_struct *str);

/**
* @brief function copy one string to another correctly
* @param str_to_copy string to copy
* @param copy_here string, where to copy
* @return STR_OK(1) in case of success, STR_ERR(0) else
*/
int string_copy(string_struct *str_to_copy, string_struct *copy_here);

int add_char_to_string(string_struct *str, int c);

/**
 * @brief Add string to a string structure, primary used for code generation
 * @param str Pointer on structure
 * @param str_to_add String that will be added
 * @return STR_OK(1) in case of success, STR_ERR(0) else
 */
int add_string_to_string(string_struct *str, const char *str_to_add);

#endif
