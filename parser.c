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
#include "scanner.h"

/* Global variables*/
int SCANNER_RESPONSE;   // for return value from get_token()
int PARAMETERS_NUMBER;
/*-----------------*/

/**
 * @brief
 * @param data code struct data
 * @return some number TODO
 * */
int token_check(code_struct *data) {
    SCANNER_RESPONSE = get_token(&data->tok);
    if (data->tok.attribute == KEYWORD_REQUIRE) {
        init_code_struct(data);
    }
}

void init_code_struct(code_struct *data) {

}


int get_token_check(code_struct *data) {

    return (get_token(&data->tok) != OK) ? false : true;
}

int token_type_check(code_struct *data, token_type type) {

    return (data->tok.type != type) ? false : true;
}

int token_check_keyword(code_struct *data, keyword_type type) {
    
    return ((data->tok.type != TOKEN_KEYWORD) || (data->tok.attribute.keyword != type)) ? false : true;
}
