/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Grammar
 * @author Aleksandr Verevkin (xverev00)
 */

#include <stdio.h>

#include "grammar.h"
#include "exp_stack.h"
#include "scanner.h"
#include "error.h"

stack_t stack;

typedef enum {

    T_PLUS,     // +
    T_MINUS,    // =
    T_DIV,      // /
    T_MUL,      // *
    T_IDIV,     // // 
    T_RBR,      // (
    T_LBR,      // )
    T_ID,       // id, int, str, double
    T_LT,      // <
    T_GT,      // >
    T_LEQ,      // <=
    T_GEQ,      // >=
    T_EQ,       // ==
    T_NE,       // ~=
    T_SIGN      // $
} prec_enum;

char precedence_tab[15][15] = {
//    +    -    /    *    //   (    )    id   <    >    <=   >=   ==   ~=   $
    {'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' },  // +
    {'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' },  // -
    {'<' ,'<' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'<' ,'<', '<' ,'<' ,'<' ,'<' ,'<' },  // /
    {'<' ,'<' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'<' ,'<', '<' ,'<' ,'<' ,'<' ,'<' },  // *
    {'<' ,'<' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'<' ,'<', '<' ,'<' ,'<' ,'<' ,'<' },  // //
    {'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'N' ,'N' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' },  // (
    {'>' ,'>' ,'>' ,'>' ,'>' ,'=' ,'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'#' },  // )
    {'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'N' ,'N' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' ,'<' },  // id
    {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'<' },  // <
    {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'<' },  // >
    {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'<' },  // <=
    {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'<' },  // >=
    {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'<' },  // ==
    {'>' ,'>' ,'>' ,'>' ,'>' ,'<' ,'>' ,'>' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'<' },  // ~=
    {'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'#' ,'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'>' ,'#' },  // $
};

static elem_enum get_type(token_struct *tkn) {

    if (tkn->type == TOKEN_PLUS) {
        return PLUS;
    } else if (tkn->type == TOKEN_MINUS) {
        return MINUS;
    } else if (tkn->type == TOKEN_DIV) {
        return DIV;
    } else if (tkn->type == TOKEN_MUL) {
        return MUL;
    } else if (tkn->type == TOKEN_IDIV) {
        return IDIV;
    } else if (tkn->type == TOKEN_ID) {
        return ID;
    } else if (tkn->type == TOKEN_INT) {
        return INT;
    } else if (tkn->type == TOKEN_DOUBLE) {
        return DOUBLE;
    } else if (tkn->type == TOKEN_STRING) {
        return STRING;
    } else if (tkn->type == TOKEN_LESS) {
        return LT;
    } else if (tkn->type == TOKEN_GREATER) {
        return GT;
    } else if (tkn->type == TOKEN_LESS_OR_EQ) {
        return LEQ;
    } else if (tkn->type == TOKEN_GREATER_OR_EQ) {
        return GEQ;
    } else if (tkn->type == TOKEN_EQUAL) {
        return EQ;
    } else if (tkn->type == TOKEN_NOT_EQUAL) {
        return NE;
    } else if (tkn->type == TOKEN_BRACKET_ROUND_L) {
        return LBR;
    } else if (tkn->type == TOKEN_BRACKET_ROUND_R) {
        return RBR;
    } else {
        return SIGN;
    }
}

static prec_enum get_precedence(elem_enum elem) {

    if (elem == PLUS) {
        return T_PLUS;
    } else if (elem == MINUS) {
        return T_MINUS;
    } else if (elem == DIV) {
        return T_DIV;
    } else if (elem == MUL) {
        return T_MUL;
    } else if (elem == IDIV) {
        return T_IDIV;
    } else if (elem == LBR) {
        return T_LBR;
    } else if (elem == RBR) {
        return T_RBR;
    } else if ((elem == ID) || (elem == INT) || (elem == STRING) || (elem == DOUBLE)) {
        return T_ID;
    } else if (elem == LT) {
        return T_LT;
    } else if (elem == GT) {
        return T_GT;
    } else if (elem == LEQ) {
        return T_LEQ;
    } else if (elem == GEQ) {
        return T_GEQ;
    } else if (elem == EQ) {
        return T_EQ;
    } else if (elem == NE) {
        return T_NE;
    } else {
        return T_SIGN;
    }
}

static rules_enum get_rule(item_stack_t *left, item_stack_t *middle, item_stack_t *right, bool id) {

    if (id) {
        if ((left->type == ID) || (left->type == INT) || (left->type == DOUBLE) || (left->type == STRING)) {
            return ID_RULE;
        } else {
            return NO_RULE;
        }
    } else {
        if ((left->type == EXP) && (right->type == EXP)) {
            if (middle->type == PLUS) {
                return E_PLUS_E;
            } else if (middle->type == MINUS) {
                return E_MINUS_E;
            } else if (middle->type == MUL) {
                return E_MUL_E;
            } else if (middle->type == DIV) {
                return E_DIV_E;
            } else if (middle->type == IDIV) {
                return E_IDIV_E;
            } else if (middle->type == LT) {
                return E_LT_E;
            } else if (middle->type == GT) {
                return E_GT_E;
            } else if (middle->type == GEQ) {
                return E_GEQ_E;
            } else if (middle->type == LEQ) {
                return E_LEQ_E;
            } else if (middle->type == EQ) {
                return E_EQ_E;
            } else if (middle->type == NE) {
                return E_NE_E;
            } else {
                return NO_RULE;
            }
        } else if ((left->type == LBR) && (left->type == EXP) && (left->type == RBR)) {
            return BR_E_BR;
        } else {
            return NO_RULE;
        }
    }
}

static int rules_check(item_stack_t *left, item_stack_t *middle, item_stack_t *right, rules_enum rule, value_type *type) {

    if ((rule == E_PLUS_E) || (rule == E_MINUS_E) || (rule == E_MUL_E)) {
        if ((left->type == TYPE_STRING) || (right->type == TYPE_STRING) || (right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)) {
            return ERR_INCOMPATIBILITY;
        } else if ((right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_UNDEF;
        } else if ((left->type == TYPE_INTEGER) && (right->type == TYPE_INTEGER)) {
            *type = TYPE_INTEGER;
            return OK;
        } else {
            //TODO convertation INT->DOUBLE
            *type = TYPE_DOUBLE;
            return OK;
        }
    } else if (rule == E_DIV_E) {
        if ((left->type == TYPE_STRING) || (right->type == TYPE_STRING) || (right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)) {
            return ERR_INCOMPATIBILITY;
        } else if ((right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_UNDEF;
        } else {
            //TODO convertation INT->DOUBLE
            *type = TYPE_DOUBLE;
            return OK;
        }
    } else if (rule == E_IDIV_E) {
        if ((left->type == TYPE_STRING) || (right->type == TYPE_STRING) || (right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)) {
            return ERR_INCOMPATIBILITY;
        } else if ((right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_UNDEF;
        } else {
            //TODO convertation DOUBLE->INT
            *type = TYPE_INTEGER;
            return OK;
        }
    } else if ((rule == E_LT_E) || (rule == E_GT_E) || (rule == E_LEQ_E) || (rule == E_GEQ_E)) {
        if ((right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)) {
            return ERR_INCOMPATIBILITY;
        } else if ((left->type == TYPE_STRING) && ((right->type == TYPE_INTEGER) || (right->type == TYPE_DOUBLE))) {
            return ERR_INCOMPATIBILITY;
        } else if (((left->type == TYPE_INTEGER) || (left->type == TYPE_DOUBLE)) && (right->type == TYPE_STRING)) {
            return ERR_INCOMPATIBILITY;
        } else if ((right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_UNDEF;
        } else {
            //TODO convertation INT->DOUBLE
            *type = TYPE_BOOL;
            return OK;
        }
    } else if ((rule == E_NE_E) || (rule == E_EQ_E)) {
        //TODO comparation with NIL
        if ((right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)) {
            return ERR_INCOMPATIBILITY;
        } else if ((left->type == TYPE_STRING) && ((right->type == TYPE_INTEGER) || (right->type == TYPE_DOUBLE))) {
            return ERR_INCOMPATIBILITY;
        } else if (((left->type == TYPE_INTEGER) || (left->type == TYPE_DOUBLE)) && (right->type == TYPE_STRING)) {
            return ERR_INCOMPATIBILITY;
        } else if ((right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_UNDEF;
        } else {
            //TODO convertation INT->DOUBLE
            *type = TYPE_BOOL;
            return OK;
        }
    } else if (rule == ID_RULE) {
        if (left->type == TYPE_NULL) {
            return ERR_UNDEF;
        } else if (left->type == TYPE_BOOL) {
            return ERR_INCOMPATIBILITY;
        } else {
            *type = left->type;
            return OK;
        }
    } else if (rule == BR_E_BR) {
        if (middle->type == TYPE_NULL) {
            return ERR_UNDEF;
        } else {
            *type = middle->type;
            return OK;
        }
    } else {
        return OK;
    }
}

//TODO expression handling
