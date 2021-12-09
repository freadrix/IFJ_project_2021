/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Syntactic and semantic check
 * @authors Matej Alexej Helc (xhelcm00), Anton Medvedev (xmedve04)
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#define GET_TOKEN                                                               \
SCANNER_RESPONSE = get_token(token);                                            \
if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE;                             \
if(IS_ID || (token->type == TOKEN_STRING))                                      \
if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE

// macro that we use for insert new item to table
#define INSERT_ITEM(_NAME)                                                      \
tab_item_t *(_NAME) =                                                           \
insert_element_hashtable(stack->top->table,                                     \
token->attribute.string->string);                                               \
if (inserted_item == NULL) return ERR_INTERNAL

// macro that we use for searching item in table
#define SEARCH_ITEM(_NAME, _TABLE, _STRING)                                     \
tab_item_t *(_NAME) = search_hashtable((_TABLE), (_STRING));                    \
bool is_there = false;                                                          \
if ((_NAME) != NULL) is_there = true

// macro that we use for check if token string is build-in func string
#define IS_BUILT_IN_FUNCTION                                                    \
IS_ID                                                   &&                      \
((!strcmp(token->attribute.string->string, "readi"))    ||                      \
(!strcmp(token->attribute.string->string, "reads"))     ||                      \
(!strcmp(token->attribute.string->string, "readn"))     ||                      \
(!strcmp(token->attribute.string->string, "substr"))    ||                      \
(!strcmp(token->attribute.string->string, "tointeger")) ||                      \
(!strcmp(token->attribute.string->string, "ord"))       ||                      \
(!strcmp(token->attribute.string->string, "chr"))       ||                      \
(!strcmp(token->attribute.string->string, "write")))

// macro that we use for check if token can be part of function body
#define IS_FUNCTION_BODY                                                        \
IS_ID                                                   ||                      \
((token->type == TOKEN_KEYWORD)                         &&                      \
((token->attribute.keyword == KEYWORD_IF)               ||                      \
(token->attribute.keyword == KEYWORD_WHILE)             ||                      \
(token->attribute.keyword == KEYWORD_LOCAL)             ||                      \
(token->attribute.keyword == KEYWORD_RETURN)))

// macro we use for call function which automatically check return value
#define CALL(_FUNCTION)                                                         \
PARSER_RESPONSE = (_FUNCTION);                                                  \
if (PARSER_RESPONSE != OK) return PARSER_RESPONSE

// macro that we use for check if keyword is function
#define IS_FUNCTION                                                             \
((token->type == TOKEN_KEYWORD) &&                                              \
(token->attribute.keyword == KEYWORD_FUNCTION))

// macro that we use for check if keyword of token is type
#define IS_TYPE                                                                 \
(token->type == TOKEN_KEYWORD)                          &&                      \
((token->attribute.keyword == KEYWORD_INTEGER)          ||                      \
(token->attribute.keyword == KEYWORD_NUMBER)            ||                      \
(token->attribute.keyword == KEYWORD_STRING)            ||                      \
(token->attribute.keyword == KEYWORD_NIL))

// macro that we use for check if token is ID
#define IS_ID (token->type == TOKEN_ID)

// macro that we use for check if there is valid expression after keyword return
#define IS_VALID                                                                \
(IS_ID                                                  ||                      \
(token->type == TOKEN_INT)                              ||                      \
(token->type == TOKEN_DOUBLE)                           ||                      \
(token->type == TOKEN_STRING))

//expression_type = (tab_item_data_type)malloc(sizeof(tab_item_data_type));
// macro we need for allocate memory
#define ALLOC                                                                   \
token = (token_struct *)malloc(sizeof(token_struct));                           \
stack = (data_stack_t *)malloc(sizeof(data_stack_t));                           \
init_data_stack(stack);                                                         \
if (!(string_init(&string))) return ERR_INTERNAL;                               \
define_working_str(&string)

// macro we need for memory free
#define CLEAN                                                                   \
string_free(&string);                                                           \
empty_data_stack(stack);                                                        \
free(token);                                                                    \
free(stack)

// macro we need for check if keyword is global
#define IS_GLOBAL                                                               \
((token->type == TOKEN_KEYWORD) &&                                              \
(token->attribute.keyword == KEYWORD_GLOBAL))

// macro we use for search variable in tables
#define SEARCH_VARIABLE_IN_ALL_TABLES(_NAME)                                    \
frame = stack->top;                                                             \
tab_item_t *(_NAME);                                                            \
while (frame != global_frame) {                                                 \
(_NAME) = search_hashtable(frame->table, token->attribute.string->string);      \
if ((_NAME) != NULL) break;                                                     \
frame = frame->previous;                                                        \
}                                                                               \
if ((_NAME) == NULL) return ERR_SEMANTIC_DEF

int SCANNER_RESPONSE;                   // for return value from get_token()
int PARSER_RESPONSE;                    // for return value from parser functions
token_struct *token;                    // token
data_stack_t *stack;                    // stack
string_struct string;                   // string
tab_item_data_type expression_type;    // expression type
int if_counter;
int while_counter;
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
    if_counter = 0;
    while_counter = 0;

    /// require "ifj21"
    PARSER_RESPONSE = start_program_parser();
    if (PARSER_RESPONSE != OK){
        CLEAN;
        return PARSER_RESPONSE;
    }
    GET_TOKEN;
    while (token->type != TOKEN_EOF) {
        if (IS_GLOBAL) {  /// <program>       -> global id : function ( <params_g> ) <rets>
            while (IS_GLOBAL) {
                PARSER_RESPONSE = global_parser();
                if(PARSER_RESPONSE != OK) {
                    CLEAN;
                    return PARSER_RESPONSE;
                }
            }
        }
        if (IS_FUNCTION) { /// <program>       -> function id ( <params> ) <rets> <state_l> end
            PARSER_RESPONSE = function_parser();
            if(PARSER_RESPONSE != OK) {
                CLEAN;
                return PARSER_RESPONSE;
            }
        }
        if (IS_ID) {      /// <program>       -> <call>
            if (!strcmp(token->attribute.string->string, "write")) {
                write_func_parser();
                GET_TOKEN;
                continue;
            }
            PARSER_RESPONSE = call_check_parser();
            if(PARSER_RESPONSE != OK) {
                CLEAN;
                return PARSER_RESPONSE;
            }
        }
        GET_TOKEN;
    }

    code_generate_main_end();
    code_write_out(stdout);
    CLEAN;
    return OK;
}

/**
 * <header>     -> require "ifj21" <program>
 */
int start_program_parser() {
    GET_TOKEN;
    if ((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_REQUIRE)) {
        GET_TOKEN;
        if ((token->type == TOKEN_STRING) && (!strcmp(token->attribute.string->string, "ifj21"))) {
            if (!push_data_item(stack)) return ERR_INTERNAL;
            if (!(code_generator_init()) || !(code_generate_main_start())) {
                return ERR_INTERNAL;
            }
            return OK;
        } else return ERR_SYNTAX;
    } else return ERR_SYNTAX;
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
            if (function_item->data->defined)
                return ERR_SEMANTIC_DEF;
            else
                function_item->data->defined = true;
        } else {
            INSERT_ITEM(inserted_item);
            inserted_item->data->item_id_type = FUNCTION;
            inserted_item->data->defined = true;
        }
    } else return ERR_SYNTAX;
    SEARCH_ITEM(function_item, stack->top->table, token->attribute.string->string);
    if (!is_there) return ERR_SYNTAX;
    /// generating function header (label, pushframe)
    if (!(code_generate_function_start(token->attribute.string->string))) {
        return ERR_INTERNAL;
    }
    // check params fully work
    CALL(function_params_parser(function_item));            /// ( <params> )

    GET_TOKEN;
    if (token->type == TOKEN_DDOT) {
        CALL(function_rets_parser(function_item)); /// <data_type> <ret>
    } else if (function_item->data->item_returns.count_returns != 0) return ERR_SEMANTIC_PARRET;

    if ((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_END)) {

        if (!(code_generate_function_end(function_item->key))) {
            return ERR_INTERNAL;
        }
        return OK;
    }
    if (IS_FUNCTION_BODY) { /// <state_l> function body
        CALL(function_body_parser(function_item));
    } else {
        return ERR_SYNTAX;
    }
    if (!(code_generate_function_end(function_item->key))) {
        return ERR_INTERNAL;
    }
    return OK;
}

/**
 * ( <params> )
 * */
int function_params_parser(tab_item_t *function_item) {
    GET_TOKEN;  /// '('
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    GET_TOKEN;  /// should be ID
    if(!push_data_item(stack)) return ERR_INTERNAL;
    int count_declare_parameters = function_item->data->item_parameters.count_parameters;
    int i;
    if(!(code_generate_empty_variables_frame())) {
        return ERR_INTERNAL;
    }
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; ++i) {
        if ((i % 2) == 0) { /// there have to be ID when i is even (0,2,4 ...) possition
            if (IS_ID) {
                if (IS_BUILT_IN_FUNCTION) return ERR_SEMANTIC_DEF;  /// check if its not build in function
                INSERT_ITEM(inserted_item); /// insert param to table
                inserted_item->data->item_id_type = VARIABLE;
                inserted_item->data->defined = true;
                GET_TOKEN;  /// should be :
                if (token->type != TOKEN_DDOT) return ERR_SYNTAX;
                GET_TOKEN;  /// should be data type
                if (IS_TYPE) {
                    if (token->attribute.keyword == KEYWORD_INTEGER) {
                        if (count_declare_parameters == 0) {
                            if (!insert_parameter_item(function_item, TYPE_INTEGER)) return ERR_SYNTAX;
                        } else {
                            if (function_item->data->item_parameters.type_parameters[(int) (i/2)] != TYPE_INTEGER)
                                return ERR_SEMANTIC_PARRET;
                        }
                        inserted_item->data->item_data_type = TYPE_INTEGER;
                    } else if (token->attribute.keyword == KEYWORD_NUMBER) {
                        if (count_declare_parameters == 0) {
                            if (!insert_parameter_item(function_item, TYPE_DOUBLE)) return ERR_SYNTAX;
                        } else {
                            if (function_item->data->item_parameters.type_parameters[(int) (i/2)] != TYPE_DOUBLE)
                                return ERR_SEMANTIC_PARRET;
                        }
                        inserted_item->data->item_data_type = TYPE_DOUBLE;
                    } else if (token->attribute.keyword == KEYWORD_STRING) {
                        if (count_declare_parameters == 0) {
                            if (!insert_parameter_item(function_item, TYPE_STRING)) return ERR_SYNTAX;
                        } else {
                            if (function_item->data->item_parameters.type_parameters[(int) (i/2)] != TYPE_STRING)
                                return ERR_SEMANTIC_PARRET;
                        }
                        inserted_item->data->item_data_type = TYPE_STRING;
                    } else if (token->attribute.keyword == KEYWORD_NIL) {
                        if (count_declare_parameters == 0) {
                            if (!insert_parameter_item(function_item, TYPE_NULL)) return ERR_SYNTAX;
                        } else {
                            if (function_item->data->item_parameters.type_parameters[(int) (i/2)] != TYPE_NULL)
                                return ERR_SEMANTIC_PARRET;
                        }
                        inserted_item->data->item_data_type = TYPE_NULL;
                    }
                } else {
                    return ERR_SYNTAX;
                }
                if (!(code_generate_variable_create(inserted_item->key))) {
                    return ERR_INTERNAL;
                }
                if (!(code_generate_save_param(inserted_item->key, i / 2 + 1))) {
                    return ERR_INTERNAL;
                }
            } else {
                return ERR_SYNTAX;
            }
        } else {    /// there have to be comma when i is odd (1, 3, 5 ...) possition
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    if ((i > 0) && ((i % 2) == 0)) return ERR_SYNTAX;
    if ((count_declare_parameters != 0) && ((count_declare_parameters - 1) != (int) (i/2)))
        return ERR_SEMANTIC_PARRET;
    return OK;
}

/**
 * <rets>
 * */
int function_rets_parser(tab_item_t *function_item) {
    GET_TOKEN;  /// should be some keyword data type
    if(!IS_TYPE) return ERR_SYNTAX;
    int count_declare_returns = function_item->data->item_returns.count_returns;
    int i;
    for (i = 0; ((token->type == TOKEN_COMMA) || (IS_TYPE)); ++i) {
        if ((i % 2) == 0) {
            if (IS_TYPE) {
                if (token->attribute.keyword == KEYWORD_INTEGER) {
                    if (count_declare_returns == 0) {
                        if (!insert_return_item(function_item, TYPE_INTEGER)) return ERR_SYNTAX;
                    } else {
                        if (function_item->data->item_returns.type_returns[(int) (i/2)] != TYPE_INTEGER)
                            return ERR_SEMANTIC_PARRET;
                    }
                    if (!(code_generate_retval_create(i/2+1))) {
                        return ERR_INTERNAL;
                    }
                } else if (token->attribute.keyword == KEYWORD_NUMBER) {
                    if (count_declare_returns == 0) {
                        if (!insert_return_item(function_item, TYPE_DOUBLE)) return ERR_SYNTAX;
                    } else {
                        if (function_item->data->item_returns.type_returns[(int) (i/2)] != TYPE_DOUBLE)
                            return ERR_SEMANTIC_PARRET;
                    }
                    if (!(code_generate_retval_create(i/2+1))) {
                        return ERR_INTERNAL;
                    }
                } else if (token->attribute.keyword == KEYWORD_STRING) {
                    if (count_declare_returns == 0) {
                        if (!insert_return_item(function_item, TYPE_STRING)) return ERR_SYNTAX;
                    } else {
                        if (function_item->data->item_returns.type_returns[(int) (i/2)] != TYPE_STRING)
                            return ERR_SEMANTIC_PARRET;
                    }
                    if (!(code_generate_retval_create(i/2+1))) {
                        return ERR_INTERNAL;
                    }
                } else {
                    if (count_declare_returns == 0) {
                        if (!insert_return_item(function_item, TYPE_NULL)) return ERR_SYNTAX;
                    } else {
                        if (function_item->data->item_returns.type_returns[(int) (i/2)] != TYPE_NULL)
                            return ERR_SEMANTIC_PARRET;
                    }
                    if (!(code_generate_retval_create(i/2+1))) {
                        return ERR_INTERNAL;
                    }
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
    if ((count_declare_returns != 0) && ((count_declare_returns - 1) != (int) (i/2)))
        return ERR_SEMANTIC_PARRET;
    return OK;
}

/**
 * <body>
 * */
int function_body_parser(tab_item_t *function_item) {
    /// function (while/if) body
    while (!((token->type == TOKEN_KEYWORD) &&                        \
            ((token->attribute.keyword == KEYWORD_END) || (token->attribute.keyword == KEYWORD_ELSE)))) {
        if (token->type == TOKEN_KEYWORD) {
            if (token->attribute.keyword == KEYWORD_LOCAL) {
                CALL(def_var_parser(function_item));
                continue;
            } else if (token->attribute.keyword == KEYWORD_IF) {
                CALL(if_parser(function_item));
                GET_TOKEN;  // PROTOZE IF KONCI NA END A PAK NASTANE CHYBA POTREBUJEME NACIST DALSI TOKEN
                continue;
            } else if (token->attribute.keyword == KEYWORD_WHILE) {
                CALL(while_parser(function_item));
                GET_TOKEN;  // PROTOZE IF KONCI NA END A PAK NASTANE CHYBA POTREBUJEME NACIST DALSI TOKEN
                continue;
            } else if (token->attribute.keyword == KEYWORD_RETURN) {
                CALL(return_parser(function_item));
                continue;
            } else {
                return ERR_SYNTAX;
            }
        }
        if (IS_ID) {
            if (!(strcmp(token->attribute.string->string, "write"))) {
                write_func_parser();
                GET_TOKEN;
                continue;
            } else {
                CALL(id_in_body_parser(function_item));
                continue;
            }
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
    for (i = 0; token->type != TOKEN_KEYWORD; ++i) {
        if ((i % 2) == 0) {
            if (IS_ID) {
                if (is_function()) {
                    return ERR_SYNTAX;
                } else {
                    CALL(exp_processing(token, stack, &expression_type));
                    if ((function_item->data->item_returns.type_returns[(int) (i / 2)] == TYPE_DOUBLE) && (expression_type == TYPE_INTEGER)) {
                        if (!(code_generate_stack_convert_float_first())) {
                            return ERR_INTERNAL;
                        }
                        expression_type = TYPE_DOUBLE;
                    }
                    if (!(code_generate_pop_stack_result())) {
                        return ERR_INTERNAL;
                    }
                    if (!(code_generate_save_expression_result_on_retval(i / 2 + 1))) {
                        return ERR_INTERNAL;
                    }
                    if (function_item->data->item_returns.type_returns[(int) (i / 2)] != expression_type)
                        return ERR_SEMANTIC_PARRET;
                }
            } else {
                CALL(exp_processing(token, stack, &expression_type));
                if ((function_item->data->item_returns.type_returns[(int) (i / 2)] == TYPE_DOUBLE) && (expression_type == TYPE_INTEGER)) {
                    if (!(code_generate_stack_convert_float_first())) {
                        return ERR_INTERNAL;
                    }
                    expression_type = TYPE_DOUBLE;
                }
                if (!(code_generate_pop_stack_result())) {
                    return ERR_INTERNAL;
                }
                if (!(code_generate_save_expression_result_on_retval(i / 2 + 1))) {
                    return ERR_INTERNAL;
                }
                if (function_item->data->item_returns.type_returns[(int) (i / 2)] != expression_type)
                    return ERR_SEMANTIC_PARRET;
            }
            continue;
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    if (!(code_generate_function_return(function_item->key))) {
        return ERR_INTERNAL;
    }
    if ((i == 0) && (function_item->data->item_returns.count_returns == 0)) {
        return OK;
    } else {
        if ((i > 0) && (i % 2 == 1) && (((int) (i/2)) == function_item->data->item_returns.count_returns - 1)) {
            return OK;
        }
        if ((i > 1) && (i % 2 == 0)) {
            return ERR_SYNTAX;
        } else {
            return ERR_SEMANTIC_PARRET;
        }
    }
}

/**
 * while <conditions> do <state_l> end
 */
int while_parser(tab_item_t *function_item) {
    GET_TOKEN;
    if (token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_DO) return ERR_SYNTAX;
    if (is_function()) return ERR_SYNTAX;
    CALL(exp_processing(token, stack, &expression_type));
    if (!(code_generate_pop_stack_result())) {
        return ERR_INTERNAL;
    }
    if (!(code_generate_while_start(while_counter))) {
        return ERR_INTERNAL;
    }
    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_DO)) return ERR_SYNTAX;
    if (!push_data_item(stack)) return ERR_INTERNAL;
    GET_TOKEN;
    CALL(function_body_parser(function_item));
    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_END)) return ERR_SYNTAX;
    if (!pop_data_item(stack)) return ERR_INTERNAL;
    if (!(code_generate_while_end(while_counter))) {
        return ERR_INTERNAL;
    }
    while_counter++;
    return OK;
}

/**
 * if <conditions> then <state_l> else <stale_l> end
 */
int if_parser(tab_item_t *function_item) {
    GET_TOKEN;
    if ((token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_THEN))    // then
        return ERR_SYNTAX;
    if (is_function()) return ERR_SYNTAX;
    CALL(exp_processing(token, stack, &expression_type));
    if (!(code_generate_pop_stack_result())) {
        return ERR_INTERNAL;
    }
    if (!(code_generate_if_start(if_counter))) {
        return ERR_INTERNAL;
    }
    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_THEN))
        return ERR_SYNTAX;
    if (!push_data_item(stack)) return ERR_INTERNAL;
    GET_TOKEN;
    CALL(function_body_parser(function_item));
    if (!pop_data_item(stack)) return ERR_INTERNAL;
    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_ELSE)) return ERR_SYNTAX;
    if (!(code_generate_else(if_counter))) {
        return ERR_INTERNAL;
    }
    if (!push_data_item(stack)) return ERR_INTERNAL;
    GET_TOKEN;
    CALL(function_body_parser(function_item));
    if (!(token->type == TOKEN_KEYWORD && token->attribute.keyword == KEYWORD_END)) return ERR_SYNTAX;
    if (!pop_data_item(stack)) return ERR_INTERNAL;
    if (!(code_generate_if_end(if_counter))) {
        return ERR_INTERNAL;
    }
    if_counter++;
    return OK;
}

/**
 *  <def_var>
 * */
int def_var_parser(tab_item_t *function_item) {
    (void)function_item;
    GET_TOKEN;  ///should be ID
    if (!IS_ID) return ERR_SYNTAX;
    SEARCH_ITEM(searched_item, stack->top->table, token->attribute.string->string);
    if (is_there || is_function()) return ERR_SEMANTIC_DEF;
    INSERT_ITEM(inserted_item);
    inserted_item->data->item_id_type = VARIABLE;
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
    ///generate variable
    if (!(code_generate_variable_create(inserted_item->key))) {
        return ERR_INTERNAL;
    }
    GET_TOKEN;   /// todo konrola typu num < int muze int < num nemuze
    if (token->type == TOKEN_ASSIGN) {  /// must be assi
        GET_TOKEN;
        if (IS_ID) {
            if (is_function()) {
                item_data_stack_t *global_frame = get_global_frame_stack(stack);
                tab_item_t *function = search_hashtable(global_frame->table, token->attribute.string->string);
                if (function->data->item_returns.count_returns == 0) return ERR_SEMANTIC_PARRET;
                if (inserted_item->data->item_data_type != function->data->item_returns.type_returns[0])
                    return ERR_SEMANTIC_ASSIGNMENT;
                CALL(call_check_parser());
                if(!(code_generate_retval_on_var(inserted_item->key, 1))) {
                    return ERR_INTERNAL;
                }
//                if (inserted_item->data->item_data_type != expression_type) return ERR_SEMANTIC_ASSIGNMENT;
            } else if ((IS_BUILT_IN_FUNCTION) && (strcmp(token->attribute.string->string, "write") != 0)) {
                CALL(build_in_functions_parser(inserted_item));
                GET_TOKEN;
            } else {
                CALL(exp_processing(token, stack, &expression_type));
                if ((inserted_item->data->item_data_type == TYPE_DOUBLE) && (expression_type == TYPE_INTEGER)) {
                    if (!(code_generate_stack_convert_float_first())) {
                        return ERR_INTERNAL;
                    }
                    expression_type = TYPE_DOUBLE;
                }
                if (!(code_generate_pop_stack_result())) {
                    return ERR_INTERNAL;
                }
                if (!(code_generate_variable_save_expression(inserted_item->key))) {
                    return ERR_INTERNAL;
                }
                if (inserted_item->data->item_data_type != expression_type) return ERR_SEMANTIC_ASSIGNMENT;
            }
        } else {
            CALL(exp_processing(token, stack, &expression_type));
            if ((inserted_item->data->item_data_type == TYPE_DOUBLE) && (expression_type == TYPE_INTEGER)) {
                if (!(code_generate_stack_convert_float_first())) {
                    return ERR_INTERNAL;
                }
                expression_type = TYPE_DOUBLE;
            }
            if (!(code_generate_pop_stack_result())) {
                return ERR_INTERNAL;
            }
            if (!(code_generate_variable_save_expression(inserted_item->key))) {
                return ERR_INTERNAL;
            }
            if (inserted_item->data->item_data_type != expression_type) return ERR_SEMANTIC_ASSIGNMENT;
        }
        inserted_item->data->defined = true;
        return OK; // todo
    } else if (IS_FUNCTION_BODY || ((token->type == TOKEN_KEYWORD) && (token->attribute.keyword == KEYWORD_END))) {
        return OK;
    } else {
        return ERR_SYNTAX;
    }
}

int id_in_body_parser(tab_item_t *function_item) {
    (void)function_item;
    if (is_function()) {
        CALL(call_check_parser());
        GET_TOKEN;
    } else if ((token->type == TOKEN_ID) && (!strcmp(token->attribute.string->string, "write"))) {
        write_func_parser();
    } else {
        item_data_stack_t *global_frame = get_global_frame_stack(stack);
        item_data_stack_t *frame;
        SEARCH_VARIABLE_IN_ALL_TABLES(item);
        GET_TOKEN;
        if (token->type == TOKEN_ASSIGN) {  /// must be assi
            GET_TOKEN;
            if (IS_ID) {
                if (is_function()) {
                    tab_item_t *function = search_hashtable(global_frame->table, token->attribute.string->string);
                    if (function->data->item_returns.count_returns == 0) return ERR_SEMANTIC_PARRET;
                    if (item->data->item_data_type != function->data->item_returns.type_returns[0])
                        return ERR_SEMANTIC_ASSIGNMENT;
                    CALL(call_check_parser());
                    if (!(code_generate_retval_on_var(item->key, 1))) {
                        return ERR_INTERNAL;
                    }
                    GET_TOKEN;
                } else {
                    if ((IS_BUILT_IN_FUNCTION) && (strcmp(token->attribute.string->string, "write") != 0)) {
                        CALL(build_in_functions_parser(item));
                        GET_TOKEN;
                    } else {
                        CALL(exp_processing(token, stack, &expression_type));
                        if ((item->data->item_data_type == TYPE_DOUBLE) && (expression_type == TYPE_INTEGER)) {
                            if (!(code_generate_stack_convert_float_first())) {
                                return ERR_INTERNAL;
                            }
                            expression_type = TYPE_DOUBLE;
                        }
                        if (!(code_generate_pop_stack_result())) {
                            return ERR_INTERNAL;
                        }
                        if (!(code_generate_variable_save_expression(item->key))) {
                            return ERR_INTERNAL;
                        }
                        if (item->data->item_data_type != expression_type) return ERR_SEMANTIC_ASSIGNMENT;
                            if (item->data->item_data_type != expression_type) return ERR_SEMANTIC_ASSIGNMENT;
                    }
                }
            } else {
                CALL(exp_processing(token, stack, &expression_type));
                if ((item->data->item_data_type == TYPE_DOUBLE) && (expression_type == TYPE_INTEGER)) {
                        if (!(code_generate_stack_convert_float_first())) {
                            return ERR_INTERNAL;
                        }
                        expression_type = TYPE_DOUBLE;
                    }
                    if (!(code_generate_pop_stack_result())) {
                        return ERR_INTERNAL;
                    }
                    if (!(code_generate_variable_save_expression(item->key))) {
                        return ERR_INTERNAL;
                    }
                if (item->data->item_data_type != expression_type) return ERR_SEMANTIC_ASSIGNMENT;
            }
            item->data->defined = true;
            return OK; // todo
        } else {
            return ERR_SYNTAX;
        }
    }
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
        if (IS_FUNCTION || (token->type == TOKEN_ID) || IS_GLOBAL) {
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
 * help func for work in expression
 * */
bool is_function() {
    item_data_stack_t *global_frame = get_global_frame_stack(stack);
    SEARCH_ITEM(declarative_function, global_frame->table, token->attribute.string->string);
    return is_there;
}

/**
 * <call>    -> id ( <expressions> )
 * */
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
    if (!(code_generate_empty_variables_frame())) {
        return ERR_INTERNAL;
    }
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; i++) {
        if (i % 2 == 0) {
            if (IS_VALID) {
                if (IS_ID) {
                    while (stack_frame != NULL) {
                        argument_of_function = search_hashtable(stack_frame->table, token->attribute.string->string);
                        if (argument_of_function != NULL) break;
                        stack_frame = stack_frame->previous;
                    }
                    if (stack_frame == NULL) return ERR_SYNTAX;
                    if (argument_of_function->data->item_data_type                                          \
                        != declaration_function->data->item_parameters.type_parameters[(int) (i / 2)])
                        return ERR_SYNTAX;
                } else {
                    if (token->type == TOKEN_INT) {
                        if (declaration_function->data->item_parameters.type_parameters[(int) (i/2)] != TYPE_INTEGER)
                            return ERR_SEMANTIC_PARRET;
                    } else if (token->type == TOKEN_DOUBLE) {
                        if (declaration_function->data->item_parameters.type_parameters[(int) (i/2)] != TYPE_DOUBLE)
                            return ERR_SEMANTIC_PARRET;
                    } else if (token->type == TOKEN_STRING) {
                        if (declaration_function->data->item_parameters.type_parameters[(int) (i/2)] != TYPE_STRING)
                            return ERR_SEMANTIC_PARRET;
                    }
                }
                if (!(code_generate_function_parameter(*token, i/2 + 1))) {
                    return ERR_INTERNAL;
                }
            } else {
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
    if (((i % 2) != 0))
        return ERR_SYNTAX;
    if (((int) (i/2)) != (declaration_function->data->item_parameters.count_parameters - 1))
        return ERR_SEMANTIC_PARRET;
    if (!(code_generate_function_call(declaration_function->key))) {
        return ERR_INTERNAL;
    }
    return OK;
}

int build_in_functions_parser(tab_item_t *assign_item) {
    item_data_stack_t *global_frame = get_global_frame_stack(stack);
    item_data_stack_t *frame;

    if (!strcmp(token->attribute.string->string, "readi")) {
        if (assign_item->data->item_data_type != TYPE_INTEGER && assign_item->data->item_data_type != TYPE_DOUBLE)
            return ERR_SEMANTIC_ASSIGNMENT;
        if (!(code_generate_function_call("readi")) || !(code_generate_retval_on_var(assign_item->key, 1))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_R) return ERR_SYNTAX;
        return OK;
    } else if (!strcmp(token->attribute.string->string, "readn")) {
        if (assign_item->data->item_data_type != TYPE_DOUBLE)
            return ERR_SEMANTIC_ASSIGNMENT;
        if (!(code_generate_function_call("readn")) ||
            !(code_generate_retval_on_var(assign_item->key, 1))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_R) return ERR_SYNTAX;
        return OK;
    } else if (!strcmp(token->attribute.string->string, "reads")) {
        if (assign_item->data->item_data_type != TYPE_STRING)
            return ERR_SEMANTIC_ASSIGNMENT;
        if (!(code_generate_function_call("reads")) ||
            !(code_generate_retval_on_var(assign_item->key, 1))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_R) return ERR_SYNTAX;
        return OK;
    } else if (!strcmp(token->attribute.string->string, "tointeger")) {
        if (assign_item->data->item_data_type != TYPE_INTEGER)
            return ERR_SEMANTIC_ASSIGNMENT;
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
        GET_TOKEN;
        if (IS_ID) {
            SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
            if (id_in_buildin_functions->data->item_data_type != TYPE_DOUBLE){
                return ERR_SEMANTIC_PARRET;
            }
        } else if (token->type == TOKEN_DOUBLE) {

        } else {
            return ERR_SEMANTIC_PARRET;
        }
        if (!(code_generate_empty_variables_frame()) ||
            !(code_generate_function_parameter(*token, 0)) ||
            !(code_generate_function_call("tointeger")) ||
            !(code_generate_retval_on_var(assign_item->key, 1))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_R) return ERR_SYNTAX;
        return OK;
    } else if (!strcmp(token->attribute.string->string, "substr")) { //• function substr(s : string, i : number, j : number) : string
        if (assign_item->data->item_data_type != TYPE_STRING) return ERR_SEMANTIC_ASSIGNMENT;
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
        GET_TOKEN;
        if (IS_ID) {
            SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
            if (id_in_buildin_functions->data->item_data_type != TYPE_STRING){
                return ERR_SEMANTIC_PARRET;
            }
        } else if (token->type == TOKEN_STRING) {

        } else {
            return ERR_SEMANTIC_PARRET;
        }
        if (!(code_generate_empty_variables_frame()) || !(code_generate_function_parameter(*token, 0))) {
            return ERR_INTERNAL;
        }
        GET_TOKEN;
        if (IS_ID) {
            SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
            if (id_in_buildin_functions->data->item_data_type != TYPE_DOUBLE){
                return ERR_SEMANTIC_PARRET;
            }
        } else if (token->type == TOKEN_DOUBLE) {

        } else {
            return ERR_SEMANTIC_PARRET;
        }
        if (!(code_generate_empty_variables_frame()) || !(code_generate_function_parameter(*token, 1))) {
            return ERR_INTERNAL;
        }
        GET_TOKEN;
        if (IS_ID) {
            SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
            if (id_in_buildin_functions->data->item_data_type != TYPE_DOUBLE){
                return ERR_SEMANTIC_PARRET;
            }
        } else if (token->type == TOKEN_DOUBLE) {

        } else {
            return ERR_SEMANTIC_PARRET;
        }
        if (!(code_generate_function_parameter(*token, 2)) ||
            !(code_generate_function_call("substr")) ||
            !(code_generate_retval_on_var(assign_item->key, 1))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_R) return ERR_SYNTAX;
        return OK;
    } else if (!strcmp(token->attribute.string->string, "ord")) {   //• function ord(s : string, i : integer) : integer
        if (assign_item->data->item_data_type != TYPE_INTEGER ) return ERR_SEMANTIC_ASSIGNMENT;
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
        GET_TOKEN;
        if (IS_ID) {
            SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
            if (id_in_buildin_functions->data->item_data_type != TYPE_STRING){
                return ERR_SEMANTIC_PARRET;
            }
        } else if (token->type == TOKEN_STRING) {

        } else {
            return ERR_SEMANTIC_PARRET;
        }
        if (!(code_generate_empty_variables_frame()) ||
            !(code_generate_function_parameter(*token, 0))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (IS_ID) {
            SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
            if (id_in_buildin_functions->data->item_data_type != TYPE_INTEGER){
                return ERR_SEMANTIC_PARRET;
            }
        } else if (token->type == TOKEN_INT) {

        } else {
            return ERR_SEMANTIC_PARRET;
        }
        if (!(code_generate_function_parameter(*token, 1)) ||
            !(code_generate_function_call("ord")) ||
            !(code_generate_retval_on_var(assign_item->key, 1))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_R) return ERR_SYNTAX;
        return OK;
    } else if (!strcmp(token->attribute.string->string, "chr")) {   //• function chr(i : integer) : string
        if (assign_item->data->item_data_type != TYPE_STRING) return ERR_SEMANTIC_ASSIGNMENT;
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
        GET_TOKEN;
        if (IS_ID) {
            SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
            if (id_in_buildin_functions->data->item_data_type != TYPE_INTEGER){
                return ERR_SEMANTIC_PARRET;
            }
        } else if (token->type == TOKEN_INT) {

        } else {
            return ERR_SEMANTIC_PARRET;
        }
        if (!(code_generate_empty_variables_frame()) ||
            !(code_generate_function_parameter(*token, 0)) ||
            !(code_generate_function_call("chr")) ||
            !(code_generate_retval_on_var(assign_item->key, 1))) {
                return ERR_INTERNAL;
            }
        GET_TOKEN;
        if (token->type != TOKEN_BRACKET_ROUND_R) return ERR_SYNTAX;
        return OK;
    }
    return OK;
}

int write_func_parser() {
    item_data_stack_t *global_frame = get_global_frame_stack(stack);
    item_data_stack_t *frame;
    GET_TOKEN;
    if (token->type != TOKEN_BRACKET_ROUND_L) return ERR_SYNTAX;
    GET_TOKEN;
    int i;
    for (i = 0; token->type != TOKEN_BRACKET_ROUND_R; ++i) {
        if (i % 2 == 0) {
            if (IS_VALID) {
                if (IS_ID) {
                    SEARCH_VARIABLE_IN_ALL_TABLES(id_in_buildin_functions);
                }
                if (!(code_generate_simple_write(*token))) {
                    return ERR_INTERNAL;
                }
            } else {
                return ERR_SEMANTIC_PARRET;
            }
        } else {
            if (token->type != TOKEN_COMMA) return ERR_SYNTAX;
        }
        GET_TOKEN;
    }
    if ((i > 0) && (i % 2 == 0)) return ERR_SYNTAX;
    return OK;
}
