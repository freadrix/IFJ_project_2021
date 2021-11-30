/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Syntactic and semantic check header
 * @author Matej Alexej Helc
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

// macro that we use for get new token and check return value
#define GET_TOKEN                                               \
SCANNER_RESPONSE = get_token(token);                            \
if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE

// macro that we use for insert new item to table
#define INSERT_ITEM                                             \
tab_item_t *item = insert_element_hashtable(stack->top->table,  \
token->attribute.string->string);                               \
if (item == NULL) return ERR_INTERNAL

//
#define SEARCH_ITEM(_STRING) \
tab_item_t *item = search_hashtable(stack->top->table,          \
(_STRING));                                                     \
if (item == NULL) return ERR_INTERNAL


/* Global variables*/
int SCANNER_RESPONSE;       // for return value from get_token()
int PARSER_RESPONSE;        // for return value from parser functions
token_struct *token;        // token
data_stack_t *stack;        // stack
//table_t *table;
//table_item *table_item;
/*-----------------*/

/** Pravidlá
    <header>    -> require "ifj21" <func>/<global>
    !<func>      -> function id ( <params> ) <rets> <body> end <program>
    <params>    -> ε
    <params>    -> id : <data_type> <param>
    <param>     -> ε
    <param>     -> , id : <data_type> <param>
    <body>      -> <def_var> <state_l>
    <def_var>   -> local id : <data_type> <assign> <def_var>
    <def_var>   -> ε
    <state_l>   -> <comm> <state_l>
    <state_l>   -> ε
    !<comm>      -> id <assign>
    !<comm>      -> if <condition> then <state_l> else <stale_l> end
    !<comm>      -> while <condition> do <state_l> end
    !<comm>      -> id ( <params_in> )
    !<comm>      -> return <param_in>
    !<comm>      -> reads ()
    !<comm>      -> readi ()
    !<comm>      -> readn ()
    !<comm>      -> write (<params_in>)
    !<comm>      -> tointeger (<expression>)
    !<comm>      -> substr ()            /////
    !<comm>      -> ord ()                /////
    !<comm>      -> chr ()                /////
    !<params_in> -> ε
    !<params_in> -> id <param_in>
    !<param_in>  -> , id <param_in>
    !<param_in>  -> ε
    <rets>      -> : <data_type> <ret>
    <rets>      -> ε
    <ret>       -> , <data_type> <ret>
    <ret>       -> ε
    <data_type> -> integer
    <data_type> -> number
    <data_type> -> string
    <data_type> -> nil
    !<assign>    -> = <expression>
    !<assign>    -> ε
 */

int parser() {
    // allocate all we need to work with
    token = (token_struct *)malloc(sizeof(token_struct));
    stack = (data_stack_t *)malloc(sizeof(data_stack_t));
    init_data_stack(stack);

    // main stage
    PARSER_RESPONSE = start_program_parser();
    if(PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    GET_TOKEN;
    while (token->type != TOKEN_EOF) {
        if((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_FUNCTION)) {
            PARSER_RESPONSE = function_parser();
            if(PARSER_RESPONSE != OK) return PARSER_RESPONSE;
        }
        if((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_GLOBAL)) {
            PARSER_RESPONSE = global_parser();
            if(PARSER_RESPONSE != OK) return PARSER_RESPONSE;
        }
        GET_TOKEN;
    }

    // clear all we used
    empty_data_stack(stack);
    free(token);
    free(stack);
    return OK;
}

/**
 * <header>    -> require "ifj21" <func>
 */
int start_program_parser() {
    GET_TOKEN;
    while (token->type != TOKEN_EOF) {
        if (token->attribute.keyword == KEYWORD_REQUIRE) {
            GET_TOKEN
            if ((token->type == TOKEN_STRING) && (!strcmp(token->attribute.string->string, "ifj21"))) {
                string_free(token->attribute.string);
                if (push_data_item(stack)) printf("making first item of stack done ok\n");
                return OK;
            }
        }
        GET_TOKEN;
    }
    return ERR_SYNTAX;   //File is empty or there wasn't require keyword
}

/**
 * <func>      -> function id ( <params> ) <rets> <body> end <program>
 * */
int function_parser() {
    printf("i love u so much!\n");
    return 0;
}

/**
 * <global>    -> TODO
 * */
int global_parser() {
    GET_TOKEN;
    if (token->type != TOKEN_ID) return ERR_SYNTAX;
    INSERT_ITEM;
    GET_TOKEN;
    if (token->type != TOKEN_DDOT) return ERR_SYNTAX;


    return OK;
}

//
///** <func>      -> function id ( <params> ) <rets> <body> end <program>
// *
// *
// */
//int p_function() {
//    init_hashtable(table);
//    int function;
//    SCANNER_RESPONSE = get_token(token);
//    while (1) {
//        if (token->attribute.keyword != KEYWORD_FUNCTION)  //missing keyword function
//            return ERR_SYNTAX;
//
//        SCANNER_RESPONSE = get_token(token);
//        if (token->type != TOKEN_ID)
//            return ERR_SYNTAX;
//
//        if (!strcmp(*token->attribute.string, "main") || !strcmp(*token->attribute.string, "write") ||
//            !strcmp(*token->attribute.string, "reads") || !strcmp(*token->attribute.string, "readn") ||
//            !strcmp(*token->attribute.string, "readi") || !strcmp(*token->attribute.string, "tointeger") ||
//            !strcmp(*token->attribute.string, "substr") || !strcmp(*token->attribute.string, "ord") ||
//            !strcmp(*token->attribute.string, "chr"))
//            return ERR_UNDEF;
//
//        if (search_hashtable(table, *token->attribute.string) != NULL) {
//            delete_all_hashtable(table);
//            return ERR_UNDEF;
//        }
//
//        if ((table_item = insert_element_hashtable(table, token->attribute.string->string)) == NULL) {
//            delete_all_hashtable(table);
//            return ERR_INTERNAL;
//        }
//
//        SCANNER_RESPONSE = get_token(token);
//        if (token->type != TOKEN_BRACKET_ROUND_L) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//
//        function = p_params();
//        if (function != OK)
//            return function;
//
//        SCANNER_RESPONSE = get_token(token);
//        if (token->type == TOKEN_DDOT) {
//            function = p_rets();
//            if (function != OK)
//                return function;
//        }
//
//        function = p_body();
//        if (function != OK)
//            return function;
//
//        SCANNER_RESPONSE = get_token(token);
//        if (token->attribute.keyword != KEYWORD_END) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//        SCANNER_RESPONSE = get_token(token);   //not EOF, loading next function
//        if (token->type == TOKEN_EOF)
//            return OK;
//    }
//}
//
///** <params>    -> ε
// *  <params>    -> id : <data_type> <param>
// *  <param>    -> ε
// *  <param>    -> , id : <data_type> <param>
// *
// */
//int p_params() {
//    table_item *params;
//    SCANNER_RESPONSE = get_token(token);
//    if (token->type != TOKEN_BRACKET_ROUND_R)  //<params>    -> ε
//        return OK;
//
//    int i = 0;
//    while (1) {
//        if (token->type != TOKEN_ID) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//
//        if ((params = search_hashtable(table, table_item->key)) == NULL) {
//            delete_all_hashtable(table);
//            return ERR_INTERNAL;
//        }
//        table_item->data.type_parameter_values[i] = token->attribute.string->string;    //TODO
//
//        SCANNER_RESPONSE = get_token(token);
//        if (token->type != TOKEN_DDOT) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//
//        SCANNER_RESPONSE = get_token(token);
//        if (token->attribute.keyword != KEYWORD_INTEGER &&
//            token->attribute.keyword != KEYWORD_NUMBER &&
//            token->attribute.keyword != KEYWORD_STRING) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//        /*TODO dát. typ parametru*/
//        SCANNER_RESPONSE = get_token(token);
//        if (token->type != TOKEN_BRACKET_ROUND_R)
//            return OK;
//
//        if (token->type != TOKEN_COMMA) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//
//        SCANNER_RESPONSE = get_token(token);
//        if (i > MAX_PARAMETERS) {
//            delete_all_hashtable(table);
//            return ERR_PARAM;
//        }
//        i++;
//    }
//}
///** <rets>      -> : <data_type> <ret>
// *  <rets>      -> ε
// *  <ret>       -> , <data_type> <ret>
// *  <ret>       -> ε
// *
// */
//int p_rets() {
//    table_item *returns;
//    int i = 0;
//    while (1) {
//        SCANNER_RESPONSE = get_token(token);
//        if (token->attribute.keyword != KEYWORD_INTEGER &&
//            token->attribute.keyword != KEYWORD_NUMBER &&
//            token->attribute.keyword != KEYWORD_STRING) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//
//        if ((returns = search_hashtable(table, table_item->key)) == NULL) {
//            delete_all_hashtable(table);
//            return ERR_INTERNAL;
//        }
//        table_item->data.type_return_values[i] = token->attribute.string->string;   //TODO
//
//        SCANNER_RESPONSE = get_token(token);
//        if (token->type != TOKEN_COMMA) {
//            delete_all_hashtable(table);
//            return ERR_SYNTAX;
//        }
//        if (token->type == TOKEN_EOL)
//            return OK;
//
//        SCANNER_RESPONSE = get_token(token);
//
//        if (i > MAX_RETURN_TYPES) {
//            delete_all_hashtable(table);
//            return ERR_PARAM;
//        }
//        i++;
//    }
//}
//
///** <body>      -> <def_var> <state_l>
// *
// *
// */
//int p_body(){
//    int function;
//    while (1) {
//        SCANNER_RESPONSE = get_token(token);
//        if (token->attribute.keyword == KEYWORD_LOCAL) {
//            function = p_defvar();
//            if (function != OK) {
//                delete_all_hashtable(table);
//                return function;
//            }
//        }
//        function = p_state_l();
//        if (function != OK) {
//            delete_all_hashtable(table);
//            return function;
//        }
//    }
//}
///** <def_var>   -> local id : <data_type> <assign> <def_var>
// *  <def_var>   -> ε
// *
// */
//int p_defvar(){
//    int function;
//    SCANNER_RESPONSE = get_token(token);
//    if (token->type != TOKEN_ID) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//
//    /* TODO Lokálne premenné kam?*/
//
//    SCANNER_RESPONSE = get_token(token);
//    if (token->type != TOKEN_DDOT) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//
//    SCANNER_RESPONSE = get_token(token);
//    if (token->attribute.keyword != KEYWORD_INTEGER &&
//        token->attribute.keyword != KEYWORD_NUMBER &&
//        token->attribute.keyword != KEYWORD_STRING) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//
//    /* TODO Lokálne premenné dát. typ*/
//
//    SCANNER_RESPONSE = get_token(token);
//    if (token->type == TOKEN_ASSIGN){
//        function = p_assign();
//        return function;
//    }
//    if (token->type == TOKEN_EOL) {
//        SCANNER_RESPONSE = get_token(token);
//        return OK;
//    }
//
//    return ERR_SYNTAX;
//}
///** <assign>    -> = <expression>
// *  <assign>    -> ε
// *
// *
// */
//int p_assign(){
//    int function;
//    SCANNER_RESPONSE = get_token(token);
//    if (token->type != TOKEN_ID && token->type != TOKEN_INT &&
//        token->type != TOKEN_DOUBLE && token->type != TOKEN_STRING) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//    /*TODO vložiť hodnotu do premennej*/
//}
//
///** <state_l>   -> <comm> <state_l>
// *  <state_l>   -> ε
// *
// */
//int p_state_l() {
//    int function;
//
//    if (token->attribute.keyword == KEYWORD_WHILE) {
//        function = p_while();
//        return function;
//    } else if (token->attribute.keyword == KEYWORD_IF) {
//        function = p_if();
//        return function;
//    } else if (token->type == TOKEN_ID) {
//
//    } else if (token->attribute.keyword == KEYWORD_RETURN) {
//        function = p_params_in();
//        return function;
//    } else if (token->type == TOKEN_EOL) {
//        return OK;
//    }
//
//}
//
//int p_while(){
//    int function;
//
//    //čakám nejakú podmienku example ( i < y )
//    /* TODO WHILE*/
//    SCANNER_RESPONSE = get_token(token);
//    if (token->attribute.keyword != KEYWORD_DO) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//    SCANNER_RESPONSE = get_token(token);
//    function = p_state_l();
//    if (function != OK) {
//        delete_all_hashtable(table);
//        return function;
//    }
//
//    if (token->attribute.keyword != KEYWORD_END) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//    return OK;
//}
//
//int p_if(){
//    int function;
//    //čakám nejakú podmienku example ( i < y )
//    /* TODO IF */
//    SCANNER_RESPONSE = get_token(token);
//    if (token->attribute.keyword != KEYWORD_THEN) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//    SCANNER_RESPONSE = get_token(token);
//    function = p_state_l();
//    if (function != OK) {
//        delete_all_hashtable(table);
//        return function;
//    }
//    SCANNER_RESPONSE = get_token(token);
//    if (token->attribute.keyword != KEYWORD_ELSE) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//    SCANNER_RESPONSE = get_token(token);
//    function = p_state_l();
//    if (function != OK) {
//        delete_all_hashtable(table);
//        return function;
//    }
//    SCANNER_RESPONSE = get_token(token);
//    if (token->attribute.keyword != KEYWORD_END) {
//        delete_all_hashtable(table);
//        return ERR_SYNTAX;
//    }
//    return OK;
//}
//
//int p_params_in(){
//    /* TODO PARAMS_IN */
//}
