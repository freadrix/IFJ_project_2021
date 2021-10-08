#ifndef _STR_H_
#define _STR_H_

#include <stdlib.h>
#include <string.h>

#define STRING_LEN_INC 8

#define STR_ERR 0
#define STR_OK 1

typedef struct
{
	char *string;
    size_t length;
    size_t alloc_length;
} string_struct;

int string_init(string_struct *str);

void string_free(string_struct *str);

void string_clear(string_struct *str);

int add_char_to_string(string_struct *str, char c);

#endif
