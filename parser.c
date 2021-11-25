/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Syntactic and semantic check header
 * @author Matej Alexej Helc
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "error.h"
#include "scanner.h"

/* Global variables*/
int SCANNER_RESPONSE;   // for return value from get_token()
int PARAMETERS_NUMBER;
token_struct *token;
table_t *table;
table_item_t *tableItem;    //todo zmena
/*-----------------*/

/** Pravidlá
    <header>    -> require "ifj21" <func>
 <func>      -> function id ( <params> ) <rets> <body> end <program>
    <params>    -> ε
    <params>    -> id : <data_type> <param>
    <param>     -> ε
    <param>     -> , id : <data_type> <param>
    <body>      -> <def_var> <state_l>
    <def_var>   -> local id : <data_type> <assign> <def_var>
    <def_var>   -> ε
 <state_l>   -> <comm> <state_l>
 <state_l>   -> ε
 <comm>      -> id <assign>
 <comm>      -> if <condition> then <state_l> else <stale_l> end
 <comm>      -> while <condition> do <state_l> end
 <comm>      -> id ( <params_in> )
 <comm>      -> return <param_in>
 <comm>      -> reads ()
 <comm>      -> readi ()
 <comm>      -> readn ()
 <comm>      -> write (<params_in>)
 <comm>      -> tointeger (<expression>)
 <comm>      -> substr ()            /////
 <comm>      -> ord ()                /////
 <comm>      -> chr ()                /////
 <params_in> -> ε
 <params_in> -> id <param_in>
 <param_in>  -> , id <param_in>
 <param_in>  -> ε
    <rets>      -> : <data_type> <ret>
    <rets>      -> ε
    <ret>       -> , <data_type> <ret>
    <ret>       -> ε
 <data_type> -> integer
 <data_type> -> number
 <data_type> -> string
 <data_type> -> nil
 <assign>    -> = <expression>
 <assign>    -> ε
 */


/** <header>    -> require "ifj21" <func>
 *
 *
 */
int get_token_check() {
    token_struct *token;
    SCANNER_RESPONSE = get_token(token);
    while (true) {
        if (token->attribute.keyword == KEYWORD_REQUIRE) {
            SCANNER_RESPONSE = get_token(token);
            if (token->type == TOKEN_ID && !strcmp(*token->attribute.string, "\"ifj21\""))
                break;
        }
    }
}

/** <func>      -> function id ( <params> ) <rets> <body> end <program>
 *
 *
 */
int p_function() {
    int function;
    init_hashtab(table);
    SCANNER_RESPONSE = get_token(token));
    if (token->attribute.keyword != KEYWORD_FUNCTION)  //missing keyword function
        return ERR_SYNTAX;

    SCANNER_RESPONSE = get_token(token));
    if (token->type != TOKEN_ID)
        return ERR_SYNTAX;

    if (!strcmp(*token->attribute.string, "main") || !strcmp(*token->attribute.string, "write") ||
        !strcmp(*token->attribute.string, "reads") || !strcmp(*token->attribute.string, "readn") ||
        !strcmp(*token->attribute.string, "readi") || !strcmp(*token->attribute.string, "tointeger") ||
        !strcmp(*token->attribute.string, "substr") || !strcmp(*token->attribute.string, "ord") ||
        !strcmp(*token->attribute.string, "chr"))
        return ERR_UNDEF;

    if (/*TODO porovnanie ci sa ID nezhoduje s nazvom nejakej funkcie*/)
        return ERR_UNDEF;

    if (tableItem /*TODO insert id (key) do tabulky)*/) {
        /*error ak sa nepodarilo nahrat do tabulky */
    }

    SCANNER_RESPONSE = get_token(token);
    if (token->type != TOKEN_BRACKET_ROUND_L)
        return ERR_SYNTAX;

    function = p_params();
    if (function != OK)
        return function;

    SCANNER_RESPONSE = get_token(token);
    if (token->type == TOKEN_DDOT) {
        function = p_rets();
        if (function != OK)
            return function;
    }

    funtion = p_body();
    if (function != OK)
        return function;

}

/** <params>    -> ε
 *  <params>    -> id : <data_type> <param>
 *  <param>    -> ε
 *  <param>    -> , id : <data_type> <param>
 *
 */
int p_params() {
    SCANNER_RESPONSE = get_token(token);
    if (token->type != TOKEN_BRACKET_ROUND_R)  //<params>    -> ε
        return OK;

    while (1) {
        if (token->type != TOKEN_ID)
            /* TODO uvolnit tabulku*/
            return ERR_SYNTAX;

        if (/*TODO porovnanie ci sa ID nezhoduje s nazvom nejakej funkcie*/)
            return ERR_UNDEF;
        /*TODO vlozit ID do tabulky*/

        SCANNER_RESPONSE = get_token(token);
        if (token->type != TOKEN_DDOT) {
            /* TODO uvolnit tabulku*/
            return ERR_SYNTAX;
        }

        SCANNER_RESPONSE = get_token(token);
        if (token->attribute.keyword != KEYWORD_INTEGER &&
            token->attribute.keyword != KEYWORD_NUMBER &&
            token->attribute.keyword != KEYWORD_STRING) {
            /* TODO uvolnit tabulku*/
            return ERR_SYNTAX;
        }

        /*TODO vlozit d.typ ID do tabulky */

        SCANNER_RESPONSE = get_token(token);
        if (token->type != TOKEN_BRACKET_ROUND_R)
            return OK;

        if (token->type != TOKEN_COMMA) {
            /* TODO uvolnit tabulku*/
            return ERR_SYNTAX;
        }

        SCANNER_RESPONSE = get_token(token);
    }
}
/** <rets>      -> : <data_type> <ret>
 *  <rets>      -> ε
 *  <ret>       -> , <data_type> <ret>
 *  <ret>       -> ε
 *
 */
int p_rets() {
    while (1) {
        SCANNER_RESPONSE = get_token(token);
        if (token->attribute.keyword != KEYWORD_INTEGER &&
            token->attribute.keyword != KEYWORD_NUMBER &&
            token->attribute.keyword != KEYWORD_STRING) {
            /* TODO uvolnit tabulku*/
            return ERR_SYNTAX;
        }

        /*TODO vlozit d.typ ID do tabulky */

        SCANNER_RESPONSE = get_token(token);
        if (token->type != TOKEN_COMMA) {
            /* TODO uvolnit tabulku*/
            return ERR_SYNTAX;
        }
        if (token->type == TOKEN_EOL)
            return OK;

        SCANNER_RESPONSE = get_token(token);
    }
}

/** <body>      -> <def_var> <state_l>
 *
 *
 */
int p_body(){
    int function;
    while (1) {
        SCANNER_RESPONSE = get_token(token);
        if (token->attribute.keyword == KEYWORD_LOCAL) {
            function = p_defvar();
            if (function != OK)
                return function;
        }
        function = p_state_l();
        if (function != OK)
            return function;
    }
}
/** <def_var>   -> local id : <data_type> <assign> <def_var>
 *  <def_var>   -> ε
 *
 */
int p_defvar(){
    int function;
    SCANNER_RESPONSE = get_token(token);
    if (token->type != TOKEN_ID) {
        /* TODO uvolnit tabulku*/
        return ERR_SYNTAX;
    }
    /* TODO nahrat ID do tabulky*/

    SCANNER_RESPONSE = get_token(token);
    if (token->type != TOKEN_DDOT) {
        /* TODO uvolnit tabulku*/
        return ERR_SYNTAX;
    }

    SCANNER_RESPONSE = get_token(token);
    if (token->attribute.keyword != KEYWORD_INTEGER &&
        token->attribute.keyword != KEYWORD_NUMBER &&
        token->attribute.keyword != KEYWORD_STRING) {
        /* TODO uvolnit tabulku*/
        return ERR_SYNTAX;
    }
    /* TODO nahrat hodnotu do tabulky*/

    SCANNER_RESPONSE = get_token(token);
    if (token->type == TOKEN_ASSIGN){
        function = p_assign();
        return function;
    } else if (token->type != TOKEN_EOL){
        return ERR_SYNTAX;
    }
    return OK;
}
/** TODO
 *
 *
 */
int p_assign(){
    int function;
    SCANNER_RESPONSE = get_token(token);
    /*TODO */
}

/** <state_l>   -> <comm> <state_l>
 *  <state_l>   -> ε
 *
 */
 int p_state_l(){
     int function;

     switch (token->attribute.keyword) {    //switch zbytočný prerobiť na if else
         case KEYWORD_WHILE:
             break;
         case KEYWORD_IF:
             break;
         default:
             break;
         TOKEN_ID ! /* TODO */
     }

 }
