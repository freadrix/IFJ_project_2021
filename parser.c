/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Syntactic and semantic check header
 * @author TODO
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "error.h"


int get_token_check(code_struct *data) {

    return (get_token(&data->tok) != OK) ? false : true;
}

int token_type_check(code_struct *data, token_type type) {

    return (data->tok.type != type) ? false : true;
}

int token_check_keyword(code_struct *data, keyword_type type) {
    
    return ((data->tok.type != TOKEN_KEYWORD) || (data->tok.attribute.keyword != type)) ? false : true;
}
