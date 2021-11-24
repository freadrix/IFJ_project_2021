/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief String operations
 * @author Ivan Tsiareshkin (xtsiar00)
 */

#include "str.h"


int string_init(string_struct *str) {
    if (!(str->string = (char*)malloc(STRING_LEN_INC))) {
        return STR_ERR;
    }
    string_clear(str);
    str->alloc_length = STRING_LEN_INC;

    return STR_OK;
}

void string_free(string_struct *str) {
    free(str->string);
}

void string_clear(string_struct *str) {
    str->length = 0;
    str->string[str->length] = '\0';
}

int add_char_to_string(string_struct *str, char c) {
    if (str->length + 1 >= str->alloc_length) {
		size_t new_lengh = str->length + STRING_LEN_INC;
		if (!(str->string = (char*)realloc(str->string, new_lengh))) {
            string_clear(str);
			return STR_ERR;
		}
		str->alloc_length = new_lengh;
	}
	str->string[str->length++] = c;
	str->string[str->length] = '\0';

	return STR_OK;
}

int add_string_to_string(string_struct *str, const char *str_to_add) {
    unsigned int added_str_length = (unsigned int) strlen(str_to_add);
    if ((str->length + 1 + added_str_length) >= str->alloc_length) {
		size_t new_length = str->length + 1 + added_str_length;
		if (!(str->string = (char*)realloc(str->string, new_length))) {
			return STR_ERR;
		}
		str->alloc_length = new_length;
	}
	str->length += added_str_length;
    strcat(str->string, str_to_add);
	str->string[str->length] = '\0';

	return STR_OK;
}
