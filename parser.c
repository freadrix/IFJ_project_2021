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

// macro that we use for searching item in table
#define SEARCH_ITEM(_TABLE, _STRING)                            \
tab_item_t *item = search_hashtable((_TABLE), (_STRING));       \
bool is_there = false;                                          \
if (item != NULL) is_there = true;

// macro that we use for check if keyword of token is type
#define IS_TYPE                                                 \
(token->type == TOKEN_KEYWORD) &&                               \
((token->attribute.keyword == KEYWORD_INTEGER) ||               \
(token->attribute.keyword == KEYWORD_NUMBER)  ||                \
(token->attribute.keyword == KEYWORD_STRING)  ||                \
(token->attribute.keyword == KEYWORD_NIL))

// macro that we use for check if keyword is function
#define IS_FUNCTION                                             \
(token->type == TOKEN_KEYWORD) &&                               \
(token->attribute.keyword == KEYWORD_FUNCTION)

// macro we need for allocate memory
#define ALLOC                                                   \
token = (token_struct *)malloc(sizeof(token_struct));           \
stack = (data_stack_t *)malloc(sizeof(data_stack_t));           \
init_data_stack(stack);                                         \
if (!(string_init(&string))) return ERR_INTERNAL;               \
define_working_str(&string)

// macro we need for memory free
#define CLEAN                                                   \
string_free(&string);                                           \
empty_data_stack(stack);                                        \
free(token);                                                    \
free(stack)

// macro that we use for check if token is ID
#define IS_ID (token->type == TOKEN_ID)

// macro that we use for check if token string is build-in func string
#define IS_BUILT_IN_FUNCTION \
IS_ID                                                   &&      \
((!strcmp(token->attribute.string->string, "readi"))    ||      \
(!strcmp(token->attribute.string->string, "reads"))     ||      \
(!strcmp(token->attribute.string->string, "readn"))     ||      \
(!strcmp(token->attribute.string->string, "substr"))    ||      \
(!strcmp(token->attribute.string->string, "tointeger")) ||      \
(!strcmp(token->attribute.string->string, "ord"))       ||      \
(!strcmp(token->attribute.string->string, "chr"))       ||      \
(!strcmp(token->attribute.string->string, "write")))

/* Global variables*/
int SCANNER_RESPONSE;       // for return value from get_token()
int PARSER_RESPONSE;        // for return value from parser functions
token_struct *token;        // token
data_stack_t *stack;        // stack
/*-----------------*/

/** Pravidlá
    <header>        -> require "ifj21" <program>
    <program>       -> global id : function ( <params_g> ) <rets> <program>
    <program>       -> function id ( <params> ) <rets> <state_l> end <program>
    <program>       -> <call> <program>
    <program>       -> ε
    <params_g>      -> <data_type> <param_g>
    <params_g>      -> ε
    <param_g>       -> , <data_type> <param_g>
    <param_g>       -> ε
    <params>        -> ε
    <params>        -> id : <data_type> <param>
    <param>         -> ε
    <param>         -> , id : <data_type> <param>
    <def_var>       -> local id : <data_type> <assign>
    <def_var>       -> ε
    <state_l>       -> <comm> <state_l>
    <state_l>       -> ε
    <comm>          -> <def_var>
    <comm>          -> id <assign>
    <comm>          -> if <conditions> then <state_l> else <stale_l> end
    <comm>          -> while <conditions> do <state_l> end
    <comm>          -> <call>
    <comm>          -> return <expressions>
    <comm>          -> write ( <expressions> )
    <call>          -> id ( <expressions> )
    <call>          -> ε
    <params_in>     -> ε
    <params_in>     -> expression <param_in>
    <param_in>      -> , expression <param_in>
    <param_in>      -> ε
    <rets>          -> : <data_type> <ret>
    <rets>          -> ε
    <ret>           -> , <data_type> <ret>
    <ret>           -> ε
    <data_type>     -> integer
    <data_type>     -> number
    <data_type>     -> string
    <data_type>     -> nil
    <assign>        -> = <assigns>
    <assign>        -> ε
    <assigns>       -> <call> <expressions>
    <expressions>   -> expression <expression>
    <expression>    -> , expression <expression>
    <expression>    -> ε
    <expressions>   -> readi ()
    <expressions>   -> readn ()
    <expressions>   -> reads ()
    <expressions>   -> substr ( expression, expression, expression ) /// string, number, number : string
    <expressions>   -> tointeger ( expression )                      /// number : integer
    <expressions>   -> ord ( expression, expression )                /// string, integer : integer
    <expressions>   -> chr ( expression )                            /// integer : string
 */

int parser() {
    // allocate all we need to work with
    token = (token_struct *)malloc(sizeof(token_struct));
    stack = (data_stack_t *)malloc(sizeof(data_stack_t));
    init_data_stack(stack);

    string_struct string;
    if (!(string_init(&string))) {
        return ERR_INTERNAL;
    }
    define_working_str(&string);

    // main stage
    PARSER_RESPONSE = start_program_parser();
    if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    GET_TOKEN;
    while (token->type != TOKEN_EOF) {
        printf("%s\n", token->attribute.string->string);
        if ((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_GLOBAL)) {
            PARSER_RESPONSE = global_parser();
            if(PARSER_RESPONSE != OK) return PARSER_RESPONSE;
        }
        if (IS_FUNCTION) {
            PARSER_RESPONSE = function_parser();
            if(PARSER_RESPONSE != OK) return PARSER_RESPONSE;
        }
        if (token->type == TOKEN_ID) {
//            SEARCH_ITEM(token->attribute.string->string);
        }
        GET_TOKEN;
    }

    // clear all we used
    empty_data_stack(stack);
    string_free(&string);
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
            GET_TOKEN;
            printf("%s\n", token->attribute.string->string);
            if ((token->type == TOKEN_STRING) && (!strcmp(token->attribute.string->string, "ifj21"))) {
                if (!push_data_item(stack)) return ERR_INTERNAL;
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
    GET_TOKEN;
    if (IS_ID) {    // (function) ID
        if (IS_BUILT_IN_FUNCTION) return ERR_UNDEF;
        SEARCH_ITEM(stack->top->table, token->attribute.string->string);
        if (is_there) {
            if (item->data->defined) {
                return ERR_UNDEF;
            } else {
                item->data->defined = true;
            }
        }
        if (!push_data_item(stack)) return ERR_INTERNAL;

        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;

        GET_TOKEN;
        for (int i = 0; token->type != TOKEN_BRACKET_ROUND_R; ++i) {    //<params> maybe new func. ?
            if (token->type == TOKEN_EOL) {
                i--;
            } else if ((i % 2) == 0) {
                if (IS_ID) {
                    if (IS_BUILT_IN_FUNCTION) return ERR_UNDEF;
                    if (!(TABLE_ITEM = insert_element_hashtable(stack->top->table, token->attribute.string->string)) return ERR_INTERNAL; /*TODO*/

                    GET_TOKEN;
                    if (token->type != TOKEN_DDOT) return ERR_SYNTAX;

                    GET_TOKEN;
                    if (IS_TYPE) {
                        if (token->attribute.keyword == KEYWORD_INTEGER)
                            item->data->type_parameter_values[(int) (i / 2)] = TYPE_INTEGER;
                        else if (token->attribute.keyword == KEYWORD_NUMBER)
                            item->data->type_parameter_values[(int) (i / 2)] = TYPE_DOUBLE;
                        else if (token->attribute.keyword == KEYWORD_STRING)
                            item->data->type_parameter_values[(int) (i / 2)] = TYPE_STRING;
                        else
                            item->data->type_parameter_values[(int) (i / 2)] = TYPE_NULL;
                    } else {
                        return ERR_SYNTAX;
                    }
                } else {
                    return ERR_SYNTAX;
                }
            } else {
                if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
            }
            GET_TOKEN;
            }
        GET_TOKEN;
        if (token->type == TOKEN_DDOT) {    // <rets> maybe new func. ?
            GET_TOKEN;
            int i = 0;
            while (true) {
                if (IS_TYPE) {
                    if (token->attribute.keyword == KEYWORD_INTEGER)
                        item->data->type_parameter_values[i] = TYPE_INTEGER;
                    else if (token->attribute.keyword == KEYWORD_NUMBER)
                        item->data->type_parameter_values[i] = TYPE_DOUBLE;
                    else if (token->attribute.keyword == KEYWORD_STRING)
                        item->data->type_parameter_values[i] = TYPE_STRING;
                    else
                        item->data->type_parameter_values[i] = TYPE_NULL;
                } else {
                    return ERR_SYNTAX;
                }
                GET_TOKEN;
                if (token->type == TOKEN_COMMA) {
                    i++;
                    GET_TOKEN;
                    continue;
                } else {
                    break;
                }
            }
        }
        /*TODO <body> end*/
    } else {
        return ERR_SYNTAX;
    }
}

/**
 * <program>       -> global id : function
 * */
int global_parser() {
    GET_TOKEN;
    if (token->type != TOKEN_ID) return ERR_SYNTAX;
    INSERT_ITEM;
    GET_TOKEN;
    if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
    GET_TOKEN;
    if (IS_FUNCTION) {
        item->data->type_id = FUNCTION;
        return global_function_parser(item);
    }else {
        return ERR_SYNTAX;
    }
}

/**
 * (<params_g>) <rets> <program>
 * */
int global_function_parser(tab_item_t *item) {
    GET_TOKEN;
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    GET_TOKEN;
    for (int i = 0; token->type != TOKEN_BRACKET_ROUND_R; ++i) {
        if (token->type == TOKEN_EOL) {
            i--;
        } else if ((i % 2) == 0) {
            if (IS_TYPE) {
                if (token->attribute.keyword == KEYWORD_INTEGER)
                    item->data->type_parameter_values[(int) (i / 2)] = TYPE_INTEGER;
                else if (token->attribute.keyword == KEYWORD_NUMBER)
                    item->data->type_parameter_values[(int) (i / 2)] = TYPE_DOUBLE;
                else if (token->attribute.keyword == KEYWORD_STRING)
                    item->data->type_parameter_values[(int) (i / 2)] = TYPE_STRING;
                else
                    item->data->type_parameter_values[(int) (i / 1)] = TYPE_NULL;
            } else {
                return ERR_SYNTAX;
            }
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    GET_TOKEN;
    item->data->defined = true;
    if (token->type != TOKEN_DDOT) {
        // mozna potrebujeme vypis
        if ((IS_FUNCTION) || (token->type == TOKEN_ID)) {
            return OK;
        } else {
            return ERR_SYNTAX;
        }
    } else {
        for (int i = 0; !((IS_FUNCTION) || (token->type == TOKEN_ID)); ++i) {
            if (token->type == TOKEN_EOL) {
                i--;
            } else if ((i % 2) == 0) {
                if (IS_TYPE) {
                    if (token->attribute.keyword == KEYWORD_INTEGER)
                        item->data->type_return_values[(int) (i / 2)] = TYPE_INTEGER;
                    else if (token->attribute.keyword == KEYWORD_NUMBER)
                        item->data->type_return_values[(int) (i / 2)] = TYPE_DOUBLE;
                    else if (token->attribute.keyword == KEYWORD_STRING)
                        item->data->type_return_values[(int) (i / 2)] = TYPE_STRING;
                    else
                        item->data->type_return_values[(int) (i / 1)] = TYPE_NULL;
                } else {
                    return ERR_SYNTAX;
                }
            } else {
                if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
            }
            GET_TOKEN;
        }
        return OK;
    }
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
