/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Expression handling header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _EXPR_HANDLE_H_
#define _EXPR_HANDLE_H_

#include "exp_stack.h"
#include "symtable.h"
#include "scanner.h"
#include "error.h"
#include "gen_code.h"

/**
 * @enum Available symbols
 */
// typedef enum {
//
//     LT,         // <
//     GT,         // >
//     GEQ,        // >=
//     LEQ,        // <=
//     EQ,         // ==
//     NE,         // ~=
//     MINUS,      // -
//     PLUS,       // +
//     MUL,        // *
//     DIV,        // /
//     IDIV,       // //
//     LEN,        // #
//     CONCAT,     // ..
//
//     LBR,        // (
//     RBR,        // )
//     ID,         // identifier
//     INT,
//     DOUBLE,
//     STRING,
//
//     SIGN,       // $
//     STOP,
//     EXPR
// } elem_enum;

/**
 * @enum Rules
 */
// typedef enum {
//
//     E_LT_E,         // E < E
//     E_GT_E,         // E > E
//     E_GEQ_E,        // E >= E
//     E_LEQ_E,        // E <= E
//     E_EQ_E,         // E == E
//     E_NE_E,         // E ~= E
//     E_MINUS_E,      // E - E
//     E_PLUS_E,       // E + E
//     E_MUL_E,        // E * E
//     E_DIV_E,        // E / E
//     E_IDIV_E,       // E // E
//     E_LEN,          // # E
//     E_CONCAT_E,     // E .. E
//
//     ID_RULE,        // id
//     BR_E_BR,        // (E)
//     NO_RULE
// } rules_enum;

// static elem_enum get_type(token_struct *tkn);
//
// static prec_enum get_precedence(elem_enum elem);
//
// static rules_enum get_rule(item_stack_t *left, item_stack_t *middle, item_stack_t *right, bool id);
//
// static int rules_check(item_stack_t *left, item_stack_t *middle, item_stack_t *right, rules_enum rule, tab_item_data_type *type);
//
// int exp_processing(token_struct *token);

#endif
