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
}                                                               \
if(IS_ID || (token->type == TOKEN_STRING))                      \
printf("%s\n", token->attribute.string->string);                \
if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE

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
((token->type == TOKEN_KEYWORD)                         &&      \
((token->attribute.keyword == KEYWORD_IF)               ||      \
(token->attribute.keyword == KEYWORD_WHILE)             ||      \
(token->attribute.keyword == KEYWORD_LOCAL)))

// macro we use for call function which automatically check return value
#define CALL(_FUNCTION)                                         \
PARSER_RESPONSE = (_FUNCTION);                                  \
if (PARSER_RESPONSE != OK) return PARSER_RESPONSE

// macro that we use for check if keyword is function
#define IS_FUNCTION                                             \
((token->type == TOKEN_KEYWORD) &&                              \
(token->attribute.keyword == KEYWORD_FUNCTION))

// macro that we use for check if keyword of token is type
#define IS_TYPE                                                 \
(token->type == TOKEN_KEYWORD)                          &&      \
((token->attribute.keyword == KEYWORD_INTEGER)          ||      \
(token->attribute.keyword == KEYWORD_NUMBER)            ||      \
(token->attribute.keyword == KEYWORD_STRING)            ||      \
(token->attribute.keyword == KEYWORD_NIL))

// macro that we use for check if token is ID
#define IS_ID (token->type == TOKEN_ID)

// macro that we use for check if there is valid expression after keyword return
#define IS_VALID                                                \
(IS_ID                                                  ||      \
(token->type == TOKEN_INT)                              ||      \
(token->type == TOKEN_DOUBLE)                           ||      \
(token->type == TOKEN_STRING))


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
✓    <header>        -> require "ifj21" <program>
✓    <program>       -> global id : function ( <params_g> ) <rets> <program>
✓~    <program>       -> function id ( <params> ) <rets> <state_l> end <program>
✓~    <program>       -> <call> <program>
✓    <program>       -> ε
✓    <params_g>      -> <data_type> <param_g>
✓    <params_g>      -> ε
✓    <param_g>       -> , <data_type> <param_g>
✓    <param_g>       -> ε
✓    <params>        -> ε
✓    <params>        -> id : <data_types> <param>
✓    <param>         -> ε
✓    <param>         -> , id : <data_type> <param>
✓~    <def_var>       -> local id : <data_type> <assign>
✓    <def_var>       -> ε
✓~    <state_l>       -> <comm> <state_l>
✓    <state_l>       -> ε
✓    <comm>          -> <def_var>
✓~    <comm>          -> <ids> <assign>
✓~    <comm>          -> if <conditions> then <state_l> else <stale_l> end
✓~    <comm>          -> while <conditions> do <state_l> end
✓~    <comm>          -> <call>
✓~    <comm>          -> return <expressions>
✝     <comm>          -> write ( <expressions> )
✓~    <ids>           -> id <id>
✓~    <id>            -> , id <id>
✓~    <id>            -> ε
✓~    <call>          -> id ( <expressions> )
✓    <call>          -> ε
✓    <rets>          -> : <data_type> <ret>
✓    <rets>          -> ε
✓    <ret>           -> , <data_type> <ret>
✓    <ret>           -> ε
✓    <data_type>     -> integer
✓    <data_type>     -> number
✓    <data_type>     -> string
✓    <data_type>     -> nil
✝    <assign>        -> = <assigns>
✝    <assign>        -> ε
✝    <assigns>       -> <call> <expressions>
✝    <expressions>   -> expression <expression>
✝    <expression>    -> , expression <expression>
✝    <expression>    -> ε
✝    <expressions>   -> readi ()
✝    <expressions>   -> readn ()
✝    <expressions>   -> reads ()
✝    <expressions>   -> substr ( expression, expression, expression ) /// string, number, number : string
✝    <expressions>   -> tointeger ( expression )                      /// number : integer
✝    <expressions>   -> ord ( expression, expression )                /// string, integer : integer
✝    <expressions>   -> chr ( expression )                            /// integer : string
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
            while (IS_GLOBAL) {
                PARSER_RESPONSE = global_parser();
                printf("%d\n", PARSER_RESPONSE);
                if(PARSER_RESPONSE != OK) {
                    CLEAN;
                    return PARSER_RESPONSE;
                }
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
            PARSER_RESPONSE = call_check_parser();
            printf("%d\n", PARSER_RESPONSE);
            if(PARSER_RESPONSE != OK) {
                CLEAN;
                return PARSER_RESPONSE;
            }
        }
        GET_TOKEN;
    }

    code_generate_main_end();
    //code_write_out(stdout);   //TODO where we wanna call this function? parser.c / main.c?
    CLEAN;
    return OK;
}

/**
 * <header>     -> require "ifj21" <program>
 */
int start_program_parser() {
    // TODO musi zacinat require jako prvnim radkem
    GET_TOKEN;
    while (token->type != TOKEN_EOF) {
        if (token->attribute.keyword == KEYWORD_REQUIRE) {
            GET_TOKEN;
            if ((token->type == TOKEN_STRING) && (!strcmp(token->attribute.string->string, "ifj21"))) {
                if (!push_data_item(stack)) return ERR_INTERNAL;
                code_generator_init();
                code_generate_main_start();
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
    if (IS_ID) {    /// (function) ID
        if (IS_BUILT_IN_FUNCTION) return ERR_SEMANTIC_DEF;
        SEARCH_ITEM(function_item, stack->top->table, token->attribute.string->string);
        if (is_there) { /// checking if there is already defined function with same ad
            if (function_item->data->defined) {
                return ERR_SEMANTIC_DEF;
            } else {    /// function is only declared, not defined yet
                function_item->data->defined = true;
            }
        } else {
            INSERT_ITEM(inserted_item);
            inserted_item->data->item_id_type = FUNCTION;
        }
    } else return ERR_SYNTAX;
    SEARCH_ITEM(function_item, stack->top->table, token->attribute.string->string);
    if (!is_there) return ERR_SYNTAX;
    code_generate_function_start(token->attribute.string->string);  /// generating function header (label, pushframe)
    PARSER_RESPONSE = function_params_parser(function_item);    /// ( <params> )
    if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    GET_TOKEN;
    if (token->type == TOKEN_DDOT) {    /// if there is ':' than we expect getting ret. values
        PARSER_RESPONSE = function_rets_parser(function_item);  /// <data_type> <ret>
        if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    }
    if ((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_END)) {
        code_generate_function_end();   /// TODO doplnit function ID
        return OK;
    }
    if (IS_FUNCTION_BODY) { /// <state_l> function body
        PARSER_RESPONSE = function_body_parser(function_item);
        if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
    } else {
        return ERR_SYNTAX;
    }
    code_generate_function_end(); /// TODO doplnit function ID
    return OK;
}

/**
 * ( <params> )
 * */
// TODO zamyslet nad tim jak budou pushovat elementy zasobniku
int function_params_parser(tab_item_t *function_item) {
    GET_TOKEN;  /// '('
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    GET_TOKEN;  /// should be ID
    if(!push_data_item(stack)) return ERR_INTERNAL;
    int i;
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; ++i) {
        if ((i % 2) == 0) { /// there have to be ID when i is even (0,2,4 ...) possition
            if (IS_ID) {
                if (IS_BUILT_IN_FUNCTION) return ERR_SEMANTIC_DEF;  /// check if its not build in function
                INSERT_ITEM(inserted_item); /// insert param to table
                inserted_item->data->item_id_type = VARIABLE;
                GET_TOKEN;  /// should be :
                if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
                GET_TOKEN;  /// should be data type
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
                    } else if (token->attribute.keyword == KEYWORD_NIL) {  // TODO need to read pdf, maybe we'll need
                        if (!insert_parameter_item(function_item, TYPE_NULL)) return ERR_SYNTAX;
                        inserted_item->data->item_data_type = TYPE_NULL;
                    }
                } else {
                    return ERR_SYNTAX;
                }
            } else {
                return ERR_SYNTAX;
            }
        } else {    /// there have to be comma when i is odd (1, 3, 5 ...) possition
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    if (i == 0) return OK;
    if ((i % 2) == 0) return ERR_SYNTAX;
    return OK;
}

/**
 * <rets>
 * */
int function_rets_parser(tab_item_t *function_item) {
    GET_TOKEN;  /// should be some keyword data type
    if(!IS_TYPE) return ERR_SYNTAX;
    int i;
    for (i = 0; ((token->type == TOKEN_COMMA) || (IS_TYPE)); ++i) {
        if ((i % 2) == 0) {
            if (IS_TYPE) {
                if (token->attribute.keyword == KEYWORD_INTEGER) {
                    if (!insert_return_item(function_item, TYPE_INTEGER)) return ERR_SYNTAX;
                    code_generate_retval_create(i/2+1);
                } else if (token->attribute.keyword == KEYWORD_NUMBER) {
                    if (!insert_return_item(function_item, TYPE_DOUBLE)) return ERR_SYNTAX;
                    code_generate_retval_create(i/2+1);
                } else if (token->attribute.keyword == KEYWORD_STRING) {
                    if (!insert_return_item(function_item, TYPE_STRING)) return ERR_SYNTAX;
                    code_generate_retval_create(i/2+1);
                } else {
                    if (!insert_return_item(function_item, TYPE_NULL)) return ERR_SYNTAX;
                    code_generate_retval_create(i/2+1);
                }
            } else {
                return ERR_SYNTAX;
            }
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    if ((i % 2) == 0) return ERR_SYNTAX;
    return OK;
}

/**
 * <body>
 * */
int function_body_parser(tab_item_t *function_item) {
    /// function (while/if) body
    while (!((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_END))) {
        if (token->type == TOKEN_KEYWORD) {
            if (token->attribute.keyword == KEYWORD_LOCAL) {
                PARSER_RESPONSE = def_var_parser(function_item);
                if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
            } else if (token->attribute.keyword == KEYWORD_IF) {
//                PARSER_RESPONSE = if_parser(function_item);
                if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
            } else if (token->attribute.keyword == KEYWORD_WHILE) {
//                PARSER_RESPONSE = while_parser(function_item);
                if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
            } else if (token->attribute.keyword == KEYWORD_RETURN) {
                PARSER_RESPONSE = return_parser(function_item);
                if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
            } else {
                return ERR_SYNTAX;
            }
        } else if (IS_ID) {
            if (!(strcmp(token->attribute.string->string, "write"))) {
                /*TODO expression*/
                code_generate_write_function(); ///TODO params write(params), [0] param == pocet parametrov, ostatne params stringy (aj int, num etc values na string!!!!)
                return OK;
            }
            PARSER_RESPONSE = id_in_body_parser(function_item);
            if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
        } else {
            return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    return OK;
}

/**
 * return <expressions>
 */
int return_parser(tab_item_t *function_item) {
    /// current token == keyword return
    GET_TOKEN;  /// this token should be some expression
    int i;
    for (i = 0; IS_VALID || token->type == TOKEN_COMMA ||                            \
    (token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_NIL); ++i) {
        if ((i % 2) == 0) {
//            PARSER_RESPONSE = expresion_parser();   TODO Expression handler
             if (token->type == TOKEN_INT) {
                if (function_item->data->item_returns.type_returns[i/2] != TYPE_INTEGER) {
                    return ERR_SEMANTIC_PARRET;
                } else {
                    function_item->data->item_value.int_value = token->attribute.int_value;
                }
             }
             if (token->type == TOKEN_DOUBLE) {
                 if (function_item->data->item_returns.type_returns[i/2] != TYPE_DOUBLE) {
                     return ERR_SEMANTIC_PARRET;
                 } else {
                     function_item->data->item_value.double_value = token->attribute.double_value;
                 }
             }
             if (token->type == TOKEN_STRING) {
                 if (function_item->data->item_returns.type_returns[i/2] != TYPE_STRING) {
                     return ERR_SEMANTIC_PARRET;
                 } else {
                     function_item->data->item_value.string_value = token->attribute.string->string;
                 }
             }
             if (token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_NIL) {
                 /*TODO ???*/
             }
             return ERR_SYNTAX;
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    i--;
    if (((int) (i/2)) != (function_item->data->item_parameters.count_parameters - 1)) { ///checking if we got right ammount
        return ERR_SEMANTIC_PARRET;
    } else {
        return OK;
    }
}

/**
 * while <conditions> do <state_l> end
 */
//int while_parser(tab_item_t *function_item) {
//    //TODO Add while cycle (depends on Expr_handle output)
//    //actual token == KEYWORD_WHILE
//    //TODO call Expression_handler  //<conditions>
//    //GET_TOKEN;    want token DO
//    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_DO))  //do
//        return ERR_SYNTAX;
//     if (!push_data_item(stack)) return ERR_INTERNAL;
//    GET_TOKEN;
//    ///TODO PUSH item stack
//    if (!(IS_FUNCTION_BODY || (token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_END)))
//        return ERR_SYNTAX;
//    if (IS_FUNCTION_BODY) { // <state_l> (<body>)
//        PARSER_RESPONSE = function_body_parser(function_item);  //if it didnt return with error than actual token is TOKEN_END
//        if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
//        GET_TOKEN;  //getting next token, should be END again (end of while cycle)
//    }
//    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_END))
//        return ERR_SYNTAX;
//    ///TODO POP stack
//
//    return OK;    //end
//}

/**
 * if <conditions> then <state_l> else <stale_l> end
 */
//int if_parser(tab_item_t *function_item) {
//    //actual token == KEYWORD_IF
//    //TODO call Expression_handler  //<conditions>
//    //GET_TOKEN;    want token from IS_FUNCTION_BODY or keyword THEN
//    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_THEN))    // then
//        return ERR_SYNTAX;
//    GET_TOKEN;
//    ///TODO PUSH item stack
//    if (!(IS_FUNCTION_BODY || (token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_ELSE)))
//        return ERR_SYNTAX;
//    if (IS_FUNCTION_BODY) { // <state_l> (<body>)
//        PARSER_RESPONSE = function_body_parser(function_item);  //if it didnt return with error than actual token is TOKEN_END
//        if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
//        GET_TOKEN;  //getting next token, should be keyword ELSE
//    }
//    //////TODO POP item stack
//    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_ELSE))    // else
//        return ERR_SYNTAX;
//    ///TODO PUSH item stack
//    GET_TOKEN;
//    if (!(IS_FUNCTION_BODY || (token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_END)))
//        return ERR_SYNTAX;
//    if (IS_FUNCTION_BODY) { // <state_l> (<body>)
//        PARSER_RESPONSE = function_body_parser(function_item);  //if it didnt return with error than actual token is TOKEN_END
//        if (PARSER_RESPONSE != OK) return PARSER_RESPONSE;
//        GET_TOKEN;  //getting next token, should be END again (end of if else)
//    }
//    ///TODO POP item stack
//    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_END))
//        return ERR_SYNTAX;
//
//    return OK;    //end
//}

/**
 *  <def_var>
 * */
int def_var_parser(tab_item_t *function_item) {
    printf("%s", function_item->key);
    GET_TOKEN;  ///should be ID
    if (!IS_ID) return ERR_SYNTAX;
    SEARCH_ITEM(searched_item, stack->top->table, token->attribute.string->string);
    if (is_there) return ERR_SEMANTIC_DEF;   // TODO can exist variable with same name as function? NO!
    INSERT_ITEM(inserted_item);
    inserted_item->data->item_id_type = VARIABLE;
    code_generate_variable_create(token->attribute.string->string); ///CODEGEN variable
    GET_TOKEN; /// :
    if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
    GET_TOKEN;  /// data type
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
        int a = 5;
        printf("%d\n", a);
        /*TODO func. assign*/
        return OK; // todo
    } else if (IS_FUNCTION_BODY) {
        return OK;
    } else {
        return ERR_SYNTAX;
    }
}

int id_in_body_parser(tab_item_t *function_item) {
    printf("%s", function_item->key);
//    item_data_stack_t *global_frame = get_global_frame_stack(stack);
//    token_struct *id_token = token;
    // TODO add rule if i want to add id , id, id, = foo()
    GET_TOKEN;
    return OK;
}

/**
 * <program>    -> global id : function
 * */
int global_parser() {
    GET_TOKEN; ///should be id
    if (token->type != TOKEN_ID) return ERR_SYNTAX;
    INSERT_ITEM(inserted_item);
    GET_TOKEN;
    if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
    GET_TOKEN;
    if (IS_FUNCTION) {
        inserted_item->data->item_id_type = FUNCTION;
        CALL(global_function_parser(inserted_item));
    } else {
        return ERR_SYNTAX;
    }
    return OK;
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
    if ((i > 0) && ((i % 2) == 0)) return ERR_SYNTAX;
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
        if ((i % 2) == 0) return ERR_SYNTAX;
        return OK;
    }
}

/**
 * <call>    -> id ( <expressions> )
 * */
 // TODO NE kontroluje parametry
 // TODO na zacatku bude token leve zavorky
 // TODO code_gen
int call_check_parser() {
    item_data_stack_t *global_frame = get_global_frame_stack(stack);
    SEARCH_ITEM(declarative_function, global_frame->table, token->attribute.string->string);
    if(is_there == false) return ERR_SYNTAX;
    GET_TOKEN;
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    CALL(call_function_parser(declarative_function));
    return OK;
}

/**
 *
 * */
int call_function_parser(tab_item_t *declaration_function) {
    GET_TOKEN;
    tab_item_t *argument_of_function;
    item_data_stack_t *stack_frame = stack->top;
    int i;      // counter
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; i++) {
        if (i % 2 == 0) {
            if (IS_ID) {      // todo mozna bude expr
                while (stack_frame != NULL) {
                    argument_of_function = search_hashtable(stack_frame->table, token->attribute.string->string);
                    if (argument_of_function != NULL) break;
                    stack_frame = stack_frame->previous;
                }
                if (stack_frame == NULL) return ERR_SYNTAX;
                if (argument_of_function->data->item_data_type                                      \
                    != declaration_function->data->item_parameters.type_parameters[(int) (i/2)])
                    return ERR_SYNTAX;
            }
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    if (i == 0) {
        if (declaration_function->data->item_parameters.count_parameters == 0) return OK;
        else return ERR_SEMANTIC_PARRET;
    }
    i--;
    if (((i % 2) != 0) || (((int) (i/2)) != (declaration_function->data->item_parameters.count_parameters - 1) ))
        return ERR_SYNTAX;
    return OK;
}
