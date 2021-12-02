/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Syntactic and semantic check header
 * @authors Matej Alexej Helc, Anton Medvedev Hulk
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

/*
 * if(IS_ID || (token->type == TOKEN_STRING))
    printf("%s\n", token->attribute.string->string);
 * */
// macro that we use for get new token and check return value
#define GET_TOKEN                                               \
SCANNER_RESPONSE = get_token(token);                            \
if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE;             \
while (token->type == TOKEN_EOL) {                              \
    SCANNER_RESPONSE = get_token(token);                        \
    if(SCANNER_RESPONSE != OK) break;                           \
} if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE

// macro that we use for insert new item to table
#define INSERT_ITEM(_NAME)                                      \
tab_item_t *(_NAME) =                                           \
insert_element_hashtable(stack->top->table,                     \
token->attribute.string->string);                               \
if (inserted_item == NULL) return ERR_INTERNAL

// macro that we use for searching item in table
#define SEARCH_ITEM(_NAME, _TABLE, _STRING)                     \
tab_item_t *(_NAME) = search_hashtable((_TABLE), (_STRING));    \
bool is_there = false;                                          \
if ((_NAME) != NULL) is_there = true

// macro that we use for check if keyword of token is type
#define IS_TYPE                                                 \
(token->type == TOKEN_KEYWORD)                 &&               \
((token->attribute.keyword == KEYWORD_INTEGER) ||               \
(token->attribute.keyword == KEYWORD_NUMBER)   ||               \
(token->attribute.keyword == KEYWORD_STRING)   ||               \
(token->attribute.keyword == KEYWORD_NIL))

// macro that we use for check if token is ID
#define IS_ID (token->type == TOKEN_ID)

// macro that we use for check if token string is build-in func string
#define IS_BUILT_IN_FUNCTION                                    \
IS_ID                                                   &&      \
((!strcmp(token->attribute.string->string, "readi"))    ||      \
(!strcmp(token->attribute.string->string, "reads"))     ||      \
(!strcmp(token->attribute.string->string, "readn"))     ||      \
(!strcmp(token->attribute.string->string, "substr"))    ||      \
(!strcmp(token->attribute.string->string, "tointeger")) ||      \
(!strcmp(token->attribute.string->string, "ord"))       ||      \
(!strcmp(token->attribute.string->string, "chr"))       ||      \
(!strcmp(token->attribute.string->string, "write")))

// macro that we use for check if token can be part of function body
#define IS_FUNCTION_BODY                                        \
IS_ID                                                   ||      \
(token->type == TOKEN_KEYWORD)                          &&      \
((token->attribute.keyword == KEYWORD_IF)               ||      \
(token->attribute.keyword == KEYWORD_WHILE)             ||      \
(token->attribute.keyword == KEYWORD_LOCAL))


// macro that we use for check if keyword is function
#define IS_FUNCTION                                             \
((token->type == TOKEN_KEYWORD) &&                              \
(token->attribute.keyword == KEYWORD_FUNCTION))

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

// macro we need for check if keyword is global
#define IS_GLOBAL                                               \
((token->type == TOKEN_KEYWORD) &&                              \
(token->attribute.keyword == KEYWORD_GLOBAL))

/* Global variables*/
int SCANNER_RESPONSE;       // for return value from get_token()
int PARSER_RESPONSE;        // for return value from parser functions
token_struct *token;        // token
data_stack_t *stack;        // stack
string_struct string;       // string
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
    <expressions>   -> expression <expression>     // mozna potrebujeme spravit
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


/**
 * <header>        -> require "ifj21" <program>
 * */
int parser() {
    // allocate all we need to work with
    ALLOC;

    PARSER_RESPONSE = start_program_parser();
    printf("%d\n", PARSER_RESPONSE);
    if (PARSER_RESPONSE != OK){
        CLEAN;
        return PARSER_RESPONSE;
    }
    /// <program>
    GET_TOKEN;
    while (token->type != TOKEN_EOF) {
        if (IS_GLOBAL) {  /// <program>       -> global id : function ( <params_g> ) <rets>
            PARSER_RESPONSE = global_parser();
            printf("%d\n", PARSER_RESPONSE);
            if(PARSER_RESPONSE != OK) {
                CLEAN;
                return PARSER_RESPONSE;
            }
        }
        if (IS_FUNCTION) { /// <program>       -> function id ( <params> ) <rets> <state_l> end
            PARSER_RESPONSE = function_parser();
            printf("%d\n", PARSER_RESPONSE);
            if(PARSER_RESPONSE != OK) {
                CLEAN;
                return PARSER_RESPONSE;
            }
        }
        if (IS_ID) {      /// <program>       -> <call>
            PARSER_RESPONSE = call_function_parser();
            printf("%d\n", PARSER_RESPONSE);
            if(PARSER_RESPONSE != OK) {
                CLEAN;
                return PARSER_RESPONSE;
            }
        }
        GET_TOKEN;
    }

    CLEAN;
    return OK;
}

/**
 * <header>     -> require "ifj21" <program>
 */
int start_program_parser() {
    GET_TOKEN;
    while (token->type != TOKEN_EOF) {
        if (token->attribute.keyword == KEYWORD_REQUIRE) {
            GET_TOKEN;
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
 * <func>   -> function id ( <params> ) <rets> <body> end <program>
 * */
int function_parser() {
    GET_TOKEN;
    if (IS_ID) {    // (function) ID
        SEARCH_ITEM(function_item, stack->top->table, token->attribute.string->string);
        if (IS_BUILT_IN_FUNCTION) return ERR_SEMANTIC_DEF;
        if (is_there) {
            if (function_item->data->defined) {
                return ERR_SEMANTIC_DEF;
            } else {
                function_item->data->defined = true;
            }
        } else {
            INSERT_ITEM(inserted_item);
            inserted_item->data->item_id_type = FUNCTION;
        }
    } else return ERR_SYNTAX;
    SEARCH_ITEM(function_item, stack->top->table, token->attribute.string->string);
    if (!is_there) return ERR_SYNTAX;
    PARSER_RESPONSE = function_params_parser(function_item);
    if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    GET_TOKEN;
    if (token->type == TOKEN_DDOT) {
        PARSER_RESPONSE = function_rets_parser(function_item);
        if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    }
    if ((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_END)) {
        if (function_item->data->item_returns.count_returns != 0) return ERR_SEMANTIC_PARRET;
        return OK;
    }
    if (IS_FUNCTION_BODY) {
        PARSER_RESPONSE = function_body_parser(function_item);
        if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    } else {
        return ERR_SYNTAX;
    }
    return OK;
}

/**
 * ( <params> )
 * */
int function_params_parser(tab_item_t *function_item) {
    GET_TOKEN;
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    GET_TOKEN;
    if(!push_data_item(stack)) return ERR_INTERNAL;
    int i;
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; ++i) {
        if ((i % 2) == 0) {
            if (IS_ID) {
                if (IS_BUILT_IN_FUNCTION) return ERR_SEMANTIC_DEF;
                INSERT_ITEM(inserted_item);
                inserted_item->data->item_id_type = VARIABLE;
                GET_TOKEN;
                if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
                GET_TOKEN;
                if (IS_TYPE) {
                    if (token->attribute.keyword == KEYWORD_INTEGER) {
                        if (!insert_parameter_item(function_item, TYPE_INTEGER)) return ERR_SYNTAX;
                        inserted_item->data->item_data_type = TYPE_INTEGER;
                    } else if (token->attribute.keyword == KEYWORD_NUMBER) {
                        if (!insert_parameter_item(function_item, TYPE_DOUBLE)) return ERR_SYNTAX;
                        inserted_item->data->item_data_type = TYPE_DOUBLE;
                    } else if (token->attribute.keyword == KEYWORD_STRING) {
                        if (!insert_parameter_item(function_item, TYPE_STRING)) return ERR_SYNTAX;
                        inserted_item->data->item_data_type = TYPE_STRING;
                    } else if (token->attribute.keyword == KEYWORD_NIL) {  // TODO need wo reed pdf maybe we'll need
                        if (!insert_parameter_item(function_item, TYPE_NULL)) return ERR_SYNTAX;
                        inserted_item->data->item_data_type = TYPE_NULL;
                    }
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
    i--;
    if ((i % 2) != 0) return ERR_SYNTAX;
    return OK;
}

/**
 * <rets>
 * */
int function_rets_parser(tab_item_t *function_item) {
    GET_TOKEN;
    if(!IS_TYPE) return ERR_SYNTAX;
    int i;
    for (i = 0; ((token->type == TOKEN_COMMA) || (IS_TYPE)); ++i) {
        if (IS_TYPE) {
            if (token->attribute.keyword == KEYWORD_INTEGER) {
                if (!insert_return_item(function_item, TYPE_INTEGER)) return ERR_SYNTAX;
            } else if (token->attribute.keyword == KEYWORD_NUMBER) {
                if (!insert_return_item(function_item, TYPE_DOUBLE)) return ERR_SYNTAX;
            } else if (token->attribute.keyword == KEYWORD_STRING) {
                if (!insert_return_item(function_item, TYPE_STRING)) return ERR_SYNTAX;
            } else {
                if (!insert_return_item(function_item, TYPE_NULL)) return ERR_SYNTAX;
            }
        }
        GET_TOKEN;
    }
    i--;
    if ((i % 2) != 0) return ERR_SYNTAX;
    return OK;
}

/**
 * <body>
 * */
int function_body_parser(tab_item_t *function_item) {
    while (!((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_END))) {
        if (token->type == TOKEN_KEYWORD) {
            if (token->attribute.keyword == KEYWORD_LOCAL) {
                PARSER_RESPONSE = def_var_parser(function_item);
                if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
            }
            if (token->attribute.keyword == KEYWORD_IF) {
                PARSER_RESPONSE =
            }
        }
        GET_TOKEN;
    }
}

/**
 *
 * */
int def_var_parser(tab_item_t *function_item) {
    GET_TOKEN;
    if (!IS_ID) return ERR_SYNTAX;
    SEARCH_ITEM(searched_item, stack->top->table, token->attribute.string->string);
    if (is_there) return ERR_SEMANTIC_DEF;
    INSERT_ITEM(inserted_item);
    inserted_item->data->item_id_type = VARIABLE;
    GET_TOKEN;
    if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
    GET_TOKEN;
    if (IS_TYPE) {
        if (token->attribute.keyword == KEYWORD_INTEGER) {
            inserted_item->data->item_data_type = TYPE_INTEGER;
        } else if (token->attribute.keyword == KEYWORD_NUMBER) {
            inserted_item->data->item_data_type = TYPE_DOUBLE;
        } else if (token->attribute.keyword == KEYWORD_STRING) {
            inserted_item->data->item_data_type = TYPE_STRING;
        } else if (token->attribute.keyword == KEYWORD_NIL) {
            inserted_item->data->item_data_type = TYPE_NULL;
        }
    } else {
        return ERR_SYNTAX;
    }
    GET_TOKEN;
    if (token->type == TOKEN_ASSIGN) {
        /*TODO func. assign*/
    } else if (IS_FUNCTION_BODY) {
        return OK;
    } else {
        return ERR_SYNTAX;
    }
}

/**
 * <program>    -> global id : function
 * */
int global_parser() {
    GET_TOKEN;
    if (token->type != TOKEN_ID) return ERR_SYNTAX;
    INSERT_ITEM(inserted_item);
    GET_TOKEN;
    if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
    GET_TOKEN;
    if (IS_FUNCTION) {
        inserted_item->data->item_id_type = FUNCTION;
        return global_function_parser(inserted_item);
    } else {
        return ERR_SYNTAX;
    }
}

/**
 * (<params_g>) <rets> <program>
 * */
int global_function_parser(tab_item_t *inserted_item) {
    GET_TOKEN;
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    GET_TOKEN;
    int i;
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; ++i) {
        if ((i % 2) == 0) {
            if (IS_TYPE) {
                if (token->attribute.keyword == KEYWORD_INTEGER) {
                    if (!insert_parameter_item(inserted_item, TYPE_INTEGER)) return ERR_SYNTAX;
                } else if (token->attribute.keyword == KEYWORD_NUMBER) {
                    if (!insert_parameter_item(inserted_item, TYPE_DOUBLE)) return ERR_SYNTAX;
                } else if (token->attribute.keyword == KEYWORD_STRING) {
                    if (!insert_parameter_item(inserted_item, TYPE_STRING)) return ERR_SYNTAX;
                } else {
                    if (!insert_parameter_item(inserted_item, TYPE_NULL)) return ERR_SYNTAX;
                }
            } else {
                return ERR_SYNTAX;
            }
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    i--;
    if ((i % 2) != 0) return ERR_SYNTAX;
    GET_TOKEN;
    if (token->type != TOKEN_DDOT) {
        // mozna potrebujeme vypis
        if (IS_FUNCTION || (token->type == TOKEN_ID)) {
            return OK;
        } else {
            return ERR_SYNTAX;
        }
    } else {
        GET_TOKEN;
        for (i = 0; !(IS_FUNCTION || (token->type == TOKEN_ID) || IS_GLOBAL); ++i) {
            if ((i % 2) == 0) {
                if (IS_TYPE) {
                    if (token->attribute.keyword == KEYWORD_INTEGER) {
                        if (!insert_return_item(inserted_item, TYPE_INTEGER)) return ERR_SYNTAX;
                    } else if (token->attribute.keyword == KEYWORD_NUMBER) {
                        if (!insert_return_item(inserted_item, TYPE_DOUBLE)) return ERR_SYNTAX;
                    } else if (token->attribute.keyword == KEYWORD_STRING) {
                        if (!insert_return_item(inserted_item, TYPE_STRING)) return ERR_SYNTAX;
                    } else {
                        if (!insert_return_item(inserted_item, TYPE_NULL)) return ERR_SYNTAX;
                    }
                } else {
                    return ERR_SYNTAX;
                }
            } else {
                if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
            }
            GET_TOKEN;
        }
        i--;
        if ((i % 2) != 0) return ERR_SYNTAX;
        if (IS_GLOBAL) return global_parser();
        return OK;
    }
}

/**
 * <call>    -> id ( <expressions> )
 * */
int call_function_parser() {
    item_data_stack_t *global_frame = get_global_frame_stack(stack);
    SEARCH_ITEM(item, global_frame->table, token->attribute.string->string);
    if(is_there == false) return ERR_SYNTAX;
    tab_item_t *declarative_function = item;
    GET_TOKEN;
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    GET_TOKEN;
    tab_item_t *argument_function;
    int i;      // counter
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; i++) {
        if (i % 2 == 0) {
            if (IS_ID) {
                item_data_stack_t *stack_frame = stack->top;
                while (stack_frame != NULL) {
                    argument_function = search_hashtable(stack_frame->table, token->attribute.string->string);
                    if (argument_function != NULL) break;
                    stack_frame = stack_frame->previous;
                }
                if (stack_frame == NULL) return ERR_SYNTAX;
                if (argument_function->data->item_data_type                                      \
                    != declarative_function->data->item_parameters.type_parameters[(int) (i/2)])
                    return ERR_SYNTAX;
            }
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    i--;
    if (((i % 2) != 0) || (((int) (i/2)) != (declarative_function->data->item_parameters.count_parameters - 1) ))
        return ERR_SYNTAX;
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
