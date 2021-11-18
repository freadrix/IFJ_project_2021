/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Syntactic and semantic check
 * @author TODO
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"


typedef struct {

    token_struct tok;
    table_t local;
    table_t global;
    //TODO
} code_struct;

#endif
