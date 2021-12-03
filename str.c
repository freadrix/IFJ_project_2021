/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief String operations
 * @author Ivan Tsiareshkin (xtsiar00)
 */

#include "str.h"

int string_init(string_struct *str) {
    str->string = (char*) malloc(STRING_CONST_LENGHT * sizeof(char));
    if (!str->string) {
        return STR_ERR;
    }
    str->length = 0;
    str->string[0] = '\0';
    str->alloc_length = STRING_CONST_LENGHT;

    return STR_OK;
}

void string_free(string_struct *str) {
    if (str != NULL) {
        free(str->string);
    }
}

void string_clear(string_struct *str) {
    str->length = 0;
    str->string[str->length] = '\0';
}

int re_lenght(string_struct *str) {
    int new_length = str->length + STRING_CONST_LENGHT;
    str->string = (char*) realloc(str->string, new_length * sizeof(char));
    if (!str->string ) {
        string_clear(str);
        return STR_ERR;
    }
    str->alloc_length = new_length;
    return STR_OK;
}

int string_copy(string_struct *str_to_copy, string_struct *copy_here) {
	int len_copy = str_to_copy->length;
	if (len_copy >= copy_here->alloc_length) {
        int len_plus_esc = len_copy + 1;
		if (!(copy_here->string = (char *) realloc(copy_here->string, len_plus_esc))) {
			return STR_ERR;
		}
		copy_here->alloc_length = len_plus_esc;
	}
	strcpy(copy_here->string, str_to_copy->string);
	copy_here->length = len_copy;

	return STR_OK;
}

int add_char_to_string(string_struct *str, int c) {
    if (str->length + 1 >= str->alloc_length) {
        if(!re_lenght(str)) {
            string_clear(str);
            return STR_ERR;
        }
    }
    str->string[str->length] = c;
    (str->length)++;
    str->string[str->length] = '\0';

    return STR_OK;
}

int add_string_to_string(string_struct *str, const char *str_to_add) {
    int added_str_length = (int) strlen(str_to_add);
    if ((str->length + 1 + added_str_length) >= str->alloc_length) {
		int new_length = str->length + 1 + added_str_length;
		if (!(str->string = (char*) realloc(str->string, new_length))) {
			return STR_ERR;
		}
		str->alloc_length = new_length;
	}
	str->length += added_str_length;
    strcat(str->string, str_to_add);
	str->string[str->length] = '\0';

	return STR_OK;
}
