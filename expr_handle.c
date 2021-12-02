/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Expression handling
 * @author Aleksandr Verevkin (xverev00)
 */

#include <stdio.h>

#include "expr_handle.h"

stack_t stack;


typedef enum {

    T_PLUS_MINUS,     // +-
    T_DIV_MUL_IDIV,   // /*//
    T_RBR,            // (
    T_LBR,            // )
    T_ID,             // id, int, str, double
    T_REL,            // relations
    T_LEN,            // #
    T_CONCAT,         // ..
    T_SIGN            // $
} prec_enum;

char precedence_tab[9][9] = {
//                +-  /*//  (    )    id  rel   #   ..    $
/*   +-   */    {'>' ,'<' ,'<' ,'>' ,'<' ,'>' ,'<' ,'>' ,'>' },
/* / * // */    {'>' ,'>' ,'<' ,'>' ,'<' ,'>' ,'<' ,'>' ,'>' },
/*   (    */    {'<' ,'<' ,'<' ,'=' ,'<' ,'<' ,'<' ,'<' ,'#' },
/*    )   */    {'>' ,'>' ,'#' ,'>' ,'#' ,'>' ,'#' ,'>' ,'>' },
/*   id   */    {'>' ,'>' ,'#' ,'>' ,'#' ,'>' ,'#' ,'>' ,'>' },
/*   rel  */    {'<' ,'<' ,'<' ,'>' ,'<' ,'#' ,'<' ,'>' ,'>' },
/*    #   */    {'>' ,'>' ,'<' ,'>' ,'<' ,'>' ,'>' ,'>' ,'>' },
/*    ..  */    {'<' ,'<' ,'<' ,'>' ,'<' ,'>' ,'<' ,'>' ,'>' },
/*    $   */    {'<' ,'<' ,'<' ,'#' ,'<' ,'<' ,'<' ,'<' ,'#' },
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
    } else if (tkn->type == TOKEN_UNARY_LENGTH) {
        return LEN;
    } else if (tkn->type == TOKEN_CONCAT) {
        return CONCAT;
    } else if (tkn->type == TOKEN_BRACKET_ROUND_L) {
        return LBR;
    } else if (tkn->type == TOKEN_BRACKET_ROUND_R) {
        return RBR;
    } else {
        return SIGN;
    }
}

static prec_enum get_precedence(elem_enum elem) {

    if ((elem == PLUS) || (elem == MINUS)) {
        return T_PLUS_MINUS;
    } else if ((elem == DIV) || (elem == MUL) || (elem == IDIV)) {
        return T_DIV_MUL_IDIV;
    } else if (elem == LBR) {
        return T_LBR;
    } else if (elem == RBR) {
        return T_RBR;
    } else if ((elem == ID) || (elem == INT) || (elem == STRING) || (elem == DOUBLE)) {
        return T_ID;
    } else if ((elem == LT) || (elem == GT) || (elem == LEQ) || (elem == GEQ) || (elem == EQ) || (elem == NE)) {
        return T_REL;
    } else if (elem == LEN) {
        return T_LEN;
    } else if (elem == CONCAT) {
        return T_CONCAT;
    } else {
        return T_SIGN;
    }
}

static rules_enum get_rule(item_stack_t *left, item_stack_t *middle, item_stack_t *right, bool id) {

    if (id) {
        if ((left->elem == ID) || (left->elem == INT) || (left->elem == DOUBLE) || (left->elem == STRING)) {
            return ID_RULE;
        } else {
            return NO_RULE;
        }
    } else {
        if ((left->elem == EXPR) && (right->elem == EXPR)) {
            if (middle->elem == PLUS) {
                return E_PLUS_E;
            } else if (middle->elem == MINUS) {
                return E_MINUS_E;
            } else if (middle->elem == MUL) {
                return E_MUL_E;
            } else if (middle->elem == DIV) {
                return E_DIV_E;
            } else if (middle->elem == IDIV) {
                return E_IDIV_E;
            } else if (middle->elem == LT) {
                return E_LT_E;
            } else if (middle->elem == GT) {
                return E_GT_E;
            } else if (middle->elem == GEQ) {
                return E_GEQ_E;
            } else if (middle->elem == LEQ) {
                return E_LEQ_E;
            } else if (middle->elem == EQ) {
                return E_EQ_E;
            } else if (middle->elem == NE) {
                return E_NE_E;
            } else if (middle->elem == LEN) {
                return E_LEN;
            } else if (middle->elem == CONCAT) {
                return E_CONCAT_E;
            } else {
                return NO_RULE;
            }
        } else if ((left->elem == LBR) && (middle->elem == EXPR) && (right->elem == RBR)) {
            return BR_E_BR;
        } else {
            return NO_RULE;
        }
    }
}

static int rules_check(item_stack_t *left, item_stack_t *middle, item_stack_t *right, rules_enum rule, tab_item_data_type *type) {

    if ((rule == E_PLUS_E) || (rule == E_MINUS_E) || (rule == E_MUL_E)) {
        if ((left->type == TYPE_STRING) || (right->type == TYPE_STRING) ||
            (right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)  ||
            (right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_SEMANTIC_EXP;
        } else if ((right->type == TYPE_UNDEFINED) || (left->type == TYPE_UNDEFINED)) {
            return ERR_SEMANTIC_DEF;
        } else if ((left->type == TYPE_INTEGER) && (right->type == TYPE_INTEGER)) {
            *type = TYPE_INTEGER;
            return OK;
        } else {
            if (left->type == TYPE_INTEGER) {
                if (!(code_generate_stack_convert_float_second())) {
                    return ERR_INTERNAL;
                }
            } else if (right->type == TYPE_INTEGER) {
                if (!(code_generate_stack_convert_float_first())) {
                    return ERR_INTERNAL;
                }
            }
            *type = TYPE_DOUBLE;
            return OK;
        }
    } else if (rule == E_DIV_E) {
        if ((left->type == TYPE_STRING) || (right->type == TYPE_STRING) ||
            (right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)  ||
            (right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_SEMANTIC_EXP;
        } else if ((right->type == TYPE_UNDEFINED) || (left->type == TYPE_UNDEFINED)) {
            return ERR_SEMANTIC_DEF;
        } else {
            if (left->type == TYPE_INTEGER) {
                if (!(code_generate_stack_convert_float_second())) {
                    return ERR_INTERNAL;
                }
            }
            if (right->type == TYPE_INTEGER) {
                if (!(code_generate_stack_convert_float_first())) {
                    return ERR_INTERNAL;
                }
            }
            *type = TYPE_DOUBLE;
            return OK;
        }
    } else if (rule == E_IDIV_E) {
        if ((left->type == TYPE_STRING) || (right->type == TYPE_STRING) ||
            (right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)  ||
            (right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_SEMANTIC_EXP;
        } else if ((right->type == TYPE_UNDEFINED) || (left->type == TYPE_UNDEFINED)) {
            return ERR_SEMANTIC_DEF;
        } else {
            if (left->type == TYPE_DOUBLE) {
                if (!(code_generate_stack_convert_int_second())) {
                    return ERR_INTERNAL;
                }
            }
            if (right->type == TYPE_DOUBLE) {
                if (!(code_generate_stack_convert_int_first())) {
                    return ERR_INTERNAL;
                }
            }
            *type = TYPE_INTEGER;
            return OK;
        }
    } else if ((rule == E_LT_E) || (rule == E_GT_E) || (rule == E_LEQ_E) || (rule == E_GEQ_E)) {
        if ((right->type == TYPE_BOOL) || (left->type == TYPE_BOOL) ||
            (right->type == TYPE_NULL) || (left->type == TYPE_NULL)) {
            return ERR_SEMANTIC_EXP;
        } else if (((left->type == TYPE_STRING) && ((right->type == TYPE_INTEGER) || (right->type == TYPE_DOUBLE))) ||
                  (((left->type == TYPE_INTEGER) || (left->type == TYPE_DOUBLE)) && (right->type == TYPE_STRING))) {
            return ERR_SEMANTIC_EXP;
        } else if ((right->type == TYPE_UNDEFINED) || (left->type == TYPE_UNDEFINED)) {
            return ERR_SEMANTIC_DEF;
        } else {
            if ((left->type == TYPE_INTEGER) || (right->type == TYPE_DOUBLE)) {
                if (!(code_generate_stack_convert_float_second())) {
                    return ERR_INTERNAL;
                }
            } else if ((left->type == TYPE_DOUBLE) || (right->type == TYPE_INTEGER)) {
                if (!(code_generate_stack_convert_float_first())) {
                    return ERR_INTERNAL;
                }
            }
            *type = TYPE_BOOL;
            return OK;
        }
    } else if ((rule == E_NE_E) || (rule == E_EQ_E)) {
        if ((right->type == TYPE_BOOL) || (left->type == TYPE_BOOL)) {
            return ERR_SEMANTIC_EXP;
        } else if (((left->type == TYPE_STRING) && ((right->type == TYPE_INTEGER) || (right->type == TYPE_DOUBLE))) ||
                  (((left->type == TYPE_INTEGER) || (left->type == TYPE_DOUBLE)) && (right->type == TYPE_STRING))) {
            return ERR_SEMANTIC_EXP;
        } else if ((right->type == TYPE_UNDEFINED) || (left->type == TYPE_UNDEFINED)) {
            return ERR_SEMANTIC_DEF;
        } else {
            if ((left->type == TYPE_INTEGER) || (right->type == TYPE_DOUBLE)) {
                if (!(code_generate_stack_convert_float_second())) {
                    return ERR_INTERNAL;
                }
            } else if ((left->type == TYPE_DOUBLE) || (right->type == TYPE_INTEGER)) {
                if (!(code_generate_stack_convert_float_first())) {
                    return ERR_INTERNAL;
                }
            }
            *type = TYPE_BOOL;
            return OK;
        }
    } else if (rule == E_LEN) {
        if (middle->type != TYPE_STRING) {
            return ERR_SEMANTIC_EXP;
        } else {
            *type = TYPE_INTEGER;
        }
    } else if (rule == E_CONCAT_E) {
        if ((left->type != TYPE_STRING) || (right->type != TYPE_STRING)) {
            return ERR_SEMANTIC_EXP;
        } else {
            *type = TYPE_STRING;
        }
    } else if (rule == ID_RULE) {
        if (left->type == TYPE_UNDEFINED) {
            return ERR_SEMANTIC_DEF;
        } else if (left->type == TYPE_BOOL) {
            return ERR_SEMANTIC_EXP;
        } else {
            *type = left->type;
            return OK;
        }
    } else if (rule == BR_E_BR) {
        if (middle->type == TYPE_UNDEFINED) {
            return ERR_SEMANTIC_DEF;
        } else {
            *type = middle->type;
            return OK;
        }
    } else {
        return OK;
    }
}

int exp_processing(token_struct *token) {
    //initialize stack
    init_stack(&stack);
    //variables for given symbol and terminal on top of the stack
    elem_enum given_symbol;
    item_stack_t *stack_term;
    //push $ on top of the initialized stack
    if(!(push_stack(&stack, SIGN, TYPE_UNDEFINED))) {
        empty_stack(&stack);
        return ERR_INTERNAL;
    }
    for (bool end = false; !end;) {
        //assign variables;
        if(!(stack_term = stack_top_term(&stack))) {
            empty_stack(&stack);
            return ERR_INTERNAL;
        }
        given_symbol = get_type(&token);
        char prec_symbol = precedence_tab[get_precedence(stack_term)][get_precedence(given_symbol)];

        if (prec_symbol == '>') {

        } else if (prec_symbol == '<') {

        } else if (prec_symbol == '=') {

            // tab_item_data_type type;

            // if (token->type == TOKEN_INT) {
            //     type = TYPE_INTEGER;
            // } else if (token->type == TOKEN_DOUBLE) {
            //     type = TYPE_DOUBLE;
            // } else if (token->type == TOKEN_STRING) {
            //     type = TYPE_STRING;
            // } else if (token->type = TOKEN_ID) {
            //     //TODO find id in symtable and assign his type
            // } else {
            //     type = TYPE_UNDEFINED;
            // }

            // if(!(push_stack(&stack, given_symbol, type))) {
            //     empty_stack(&stack);
            //     return ERR_INTERNAL;
            // }



        } else if (prec_symbol == '#') {
            if ((stack_term == SIGN) && (given_symbol == SIGN)) {
                break;
            } else {
                empty_stack(&stack);
                return ERR_SYNTAX;
            }
        }
    }

    return OK;
}
