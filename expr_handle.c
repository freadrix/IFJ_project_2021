/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Expression handling
 * @author Aleksandr Verevkin (xverev00)
 */

#include <stdio.h>

#include "expr_handle.h"
#include "symtable.h"
#include "data_stack.h"

// macro that we use for get new token and check return value
#define GET_TOKEN                                               \
SCANNER_RESPONSE = get_token(token);                            \
if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE;             \
while (token->type == TOKEN_EOL) {                              \
    SCANNER_RESPONSE = get_token(token);                        \
    if(SCANNER_RESPONSE != OK) break;                           \
} if(SCANNER_RESPONSE != OK) return SCANNER_RESPONSE


stack_t *stack;
data_stack_t *data_stack;
int SCANNER_RESPONSE; // idk, but it can not take declaration from parser header

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

elem_enum get_elem(token_struct *tkn) {

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

tab_item_data_type get_elem_type(elem_enum elem, token_struct *token) {

    tab_item_t *tmp;
    if (elem == ID) { // FIXME, ??data_stack->top->table??
        tmp = search_hashtable(data_stack->top->table, token->attribute.string->string);
        if (!tmp) {
            return TYPE_UNDEFINED;
        } else {
            return tmp->data->item_data_type;
        }
    } else if (elem == INT) {
        return TYPE_INTEGER;
    } else if (elem == DOUBLE) {
        return TYPE_DOUBLE;
    } else if (elem == STRING) {
        return TYPE_STRING;
    } else {
        return TYPE_UNDEFINED;
    }
}

prec_enum get_precedence(elem_enum elem) {

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

rules_enum get_rule(item_stack_t *left, item_stack_t *middle, item_stack_t *right, bool id) {

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

int rules_check(item_stack_t *left, item_stack_t *middle, item_stack_t *right, rules_enum rule, tab_item_data_type *type) {

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
    }
    return OK;
}

int rule_reduce() {
    item_stack_t *left = NULL;
    item_stack_t *middle = NULL;
    item_stack_t *right = NULL;
    tab_item_data_type type;
    rules_enum rule;
    bool end = false;
    bool id = false;
    int i = 0;

    for (item_stack_t* tmp = stack_top_term(stack); tmp != NULL; tmp = tmp->nxt) {
        if (tmp->elem != LT) {
            end = false;
            i++;
        } else {
            end = true;
            break;
        }
    }

    if (i == 1 && end) {
        left = stack->top;
        id = true;
        rule = get_rule(left, middle, right, id);
    } else if (i == 3 && end) {
        left = stack->top->nxt->nxt;
        middle = stack->top->nxt;
        right = stack->top;
        id = false;
        rule = get_rule(left, middle, right, id);
    } else {
        return ERR_SYNTAX;
    }

    if (rule != NO_RULE) {
        if ((i = rules_check(left, middle, right, rule, &type)) != OK) {
            return i;
        }
        // TODO code generator functions
    } else {
        return ERR_SYNTAX;
    }

    return OK;
}

int exp_processing(token_struct *token) { // TODO code generator functions
    //initialize stack
    init_stack(stack);
    //variables for given symbol and terminal on top of the stack
    elem_enum given_symbol;
    item_stack_t *stack_term;
    //push $ on top of the initialized stack
    if(!(push_stack(stack, SIGN, TYPE_UNDEFINED))) {
        empty_stack(stack);
        return ERR_INTERNAL;
    }

    for (bool end = false; !end;) {

        if(!(stack_term = stack_top_term(stack))) {
            empty_stack(stack);
            return ERR_INTERNAL;
        }
        given_symbol = get_elem(token);
        char prec_symbol = precedence_tab[get_precedence(stack_term->elem)][get_precedence(given_symbol)];

        if (prec_symbol == '>') { // TESTME
            if (rule_reduce()) {
                empty_stack(stack);
                return ERR_INTERNAL;
            }

            break;
        } else if (prec_symbol == '<') { // TESTME PLS
            if (!(insert_after_top_term(stack, LT, TYPE_UNDEFINED))) {
                empty_stack(stack);
                return ERR_INTERNAL;
            }
            if(!(push_stack(stack, given_symbol, get_elem_type(given_symbol, token)))) {
                empty_stack(stack);
                return ERR_INTERNAL;
            }

            GET_TOKEN;
            break;
        } else if (prec_symbol == '=') { // TESTME PLS
            if(!(push_stack(stack, given_symbol, get_elem_type(given_symbol, token)))) {
                empty_stack(stack);
                return ERR_INTERNAL;
            }

            GET_TOKEN;
            break;
        } else if (prec_symbol == '#') { // TESTME PLS
            if ((stack_term->elem == SIGN) && (given_symbol == SIGN)) {
                end = true;
                break;
            } else {
                empty_stack(stack);
                return ERR_SYNTAX;
            }
        }
    }

    return OK;
}
