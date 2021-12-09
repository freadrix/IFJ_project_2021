/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Expression handling
 * @authors Aleksandr Verevkin (xverev00), Ivan Tsiareshkin (xtsiar00), Anton Medvedev (xmedve04)
 */

#include <stdio.h>

#include "expr_handle.h"
#include "symtable.h"

// macro that we use for get new token and check return value
#define GET_TOKEN                                               \
SCANNER_RESPONSE_EXP = get_token(token);                        \
if(SCANNER_RESPONSE_EXP != OK) return SCANNER_RESPONSE_EXP

stack_t *expr_stack;

typedef enum {

    T_PLUS_MINUS,     // +-
    T_DIV_MUL_IDIV,   // /*//
    T_LBR,            // (
    T_RBR,            // )
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
/*   rel  */    {'<' ,'<' ,'<' ,'>' ,'<' ,'#' ,'<' ,'<' ,'>' },
/*    #   */    {'>' ,'>' ,'<' ,'>' ,'<' ,'>' ,'>' ,'>' ,'>' },
/*    ..  */    {'<' ,'<' ,'<' ,'>' ,'<' ,'>' ,'<' ,'>' ,'>' },
/*    $   */    {'<' ,'<' ,'<' ,'#' ,'<' ,'<' ,'<' ,'<' ,'#' },
};

elem_enum get_elem(token_struct *tkn) {
    //assign element by its token type
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
    } else if (tkn->attribute.keyword == KEYWORD_NIL) {
        return NIL;
    } else {
        return SIGN;
    }
}

tab_item_data_type get_elem_type(elem_enum elem, token_struct *token, data_stack_t *data_stack) {
    item_data_stack_t *global_frame = get_global_frame_stack(data_stack);
    item_data_stack_t *frame = data_stack->top;
    //if element id, find it in data stack
    if (elem == ID) {
        tab_item_t *item;
        while (frame != global_frame) {
            item = search_hashtable(frame->table, token->attribute.string->string);
            if (item != NULL) break;
            frame = frame->previous;
        }
        //if item dont exist, error undefined
        if (item == NULL) {
            exit(ERR_SEMANTIC_DEF);
        }
        //if item not defined, return type nil
        if (item->data->defined == false) {
            return TYPE_NULL;
        }
        return item->data->item_data_type;
    } else if (elem == INT) {
        return TYPE_INTEGER;
    } else if (elem == DOUBLE) {
        return TYPE_DOUBLE;
    } else if (elem == STRING) {
        return TYPE_STRING;
    } else if (elem == NIL) {
        return TYPE_NULL;
    } else {
        return TYPE_UNDEFINED;
    }
}

prec_enum get_precedence(elem_enum elem) {
    //get precedence of the symbol, according to its precedence position
    if ((elem == PLUS) || (elem == MINUS)) {
        return T_PLUS_MINUS;
    } else if ((elem == DIV) || (elem == MUL) || (elem == IDIV)) {
        return T_DIV_MUL_IDIV;
    } else if (elem == LBR) {
        return T_LBR;
    } else if (elem == RBR) {
        return T_RBR;
    } else if ((elem == ID) || (elem == INT) || (elem == STRING) || (elem == DOUBLE) || (elem == NIL)) {
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

rules_enum get_rule(item_stack_t *left, item_stack_t *middle, item_stack_t *right, int count_of_elems) {
    //get rule for stack top terminal
    if (count_of_elems == 1) {
        if ((left->elem == ID) || (left->elem == INT) || (left->elem == DOUBLE) || (left->elem == STRING) || (left->elem == NIL)) {
            return ID_RULE;
        } else {
            return NO_RULE;
        }
    } else if (count_of_elems == 3) {
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
    } else if (count_of_elems == 2) {
        if (left->elem == LEN) {
            return E_LEN;
        } else {
            return NO_RULE;
        }
    } else {
        return NO_RULE;
    }
}

int rules_check(item_stack_t *left, item_stack_t *middle, item_stack_t *right, rules_enum rule, tab_item_data_type *type) {
    //check for types in the applying rules
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
        } else if (right->is_zero) {
            return ERR_DIV_ZERO;
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
        } else if (right->is_zero) {
            return ERR_DIV_ZERO;
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
            if ((left->type == TYPE_INTEGER) && (right->type == TYPE_DOUBLE)) {
                if (!(code_generate_stack_convert_float_second())) {
                    return ERR_INTERNAL;
                }
            } else if ((left->type == TYPE_DOUBLE) && (right->type == TYPE_INTEGER)) {
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
            if ((left->type == TYPE_INTEGER) && (right->type == TYPE_DOUBLE)) {
                if (!(code_generate_stack_convert_float_second())) {
                    return ERR_INTERNAL;
                }
            } else if ((left->type == TYPE_DOUBLE) && (right->type == TYPE_INTEGER)) {
                if (!(code_generate_stack_convert_float_first())) {
                    return ERR_INTERNAL;
                }
            }
            *type = TYPE_BOOL;
            return OK;
        }
    } else if (rule == E_LEN) {
        if (middle->type == TYPE_UNDEFINED) {
            return ERR_SEMANTIC_DEF;
        } else if (middle->type != TYPE_STRING) {
            return ERR_SEMANTIC_EXP;
        } else {
            *type = TYPE_INTEGER;
        }
    } else if (rule == E_CONCAT_E) {
        if (left->type == TYPE_UNDEFINED || right->type == TYPE_UNDEFINED) {
            return ERR_SEMANTIC_DEF;
        } else if ((left->type != TYPE_STRING) || (right->type != TYPE_STRING)) {
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

int reduce() {
    //reduce > rule
    item_stack_t *left;
    item_stack_t *middle;
    item_stack_t *right;
    left = middle = right = NULL;

    rules_enum rule;
    bool end = false;
    tab_item_data_type output_type;
    int count_of_elems = 0;
    //found count of elements until stop sign
    for (item_stack_t *iter_item = stack_top(expr_stack); iter_item != NULL; iter_item = iter_item->nxt) {
        if (iter_item->elem == STOP) {
            end = true;
            break;
        } else {
            count_of_elems++;
        }
    }
    if (count_of_elems == 1 && end) {
        //reduce by 1 element
        left = expr_stack->top;
        if ((rule = get_rule(left, middle, right, count_of_elems)) == NO_RULE) {
            return ERR_SYNTAX;
        }
    } else if (count_of_elems == 2 && end) {
        //reduce by 2 elements
        left = expr_stack->top->nxt;
        middle = expr_stack->top;
        if ((rule = get_rule(left, middle, right, count_of_elems)) == NO_RULE) {
            return ERR_SYNTAX;
        }
    } else if (count_of_elems == 3 && end) {
        //reduce by 3 elements
        left = expr_stack->top->nxt->nxt;
        middle = expr_stack->top->nxt;
        right = expr_stack->top;
        if ((rule = get_rule(left, middle, right, count_of_elems)) == NO_RULE) {
            return ERR_SYNTAX;
        }
    } else {
        return ERR_SYNTAX;
    }

    int check;
    if((check = rules_check(left, middle, right, rule, &output_type))) {
        return check;
    }

    if (!(code_generate_operations(rule))) {
        return ERR_INTERNAL;
    }
    //check if id was zero
    bool is_zero = false;
    if (rule == ID_RULE && left->is_zero) {
        is_zero = true;
    }

    for (int i = 0; i <= count_of_elems; i++) {
        pop_stack(expr_stack);
    }
    push_stack(expr_stack, EXPR, output_type, is_zero);

    return OK;
}

int exp_processing(token_struct *token, data_stack_t *data_stack, tab_item_data_type *expression_type) {
    //initialize expr_stack
    expr_stack = (stack_t *)malloc(sizeof(stack_t));
    if (!expr_stack) exit(ERR_INTERNAL);
    init_stack(expr_stack);
    //variables for given symbol and terminal on top of the expr_stack
    elem_enum given_symbol;
    item_stack_t *stack_term;
    bool is_zero;
    bool next_expr_detect = false;
    int SCANNER_RESPONSE_EXP;
    //push $ on top of the initialized stack
    if(!(push_stack(expr_stack, SIGN, TYPE_UNDEFINED, false))) {
        empty_stack(expr_stack);
        return ERR_INTERNAL;
    }
    while (true) {
        if(!(stack_term = stack_top_term(expr_stack))) {
            empty_stack(expr_stack);
            return ERR_INTERNAL;
        }
        given_symbol = get_elem(token);
        //if there is id right after the expression. without any operators, that means start of new defining
        if ((stack_term->elem == ID || stack_term->elem == INT ||
             stack_term->elem == STRING || stack_term->elem == DOUBLE || stack_term->elem == RBR) && given_symbol == ID) {
                 next_expr_detect = true;
        }
        if (next_expr_detect) {
            given_symbol = SIGN;
        }

        char prec_symbol = precedence_tab[get_precedence(stack_term->elem)][get_precedence(given_symbol)];
        //reduce prec
        if (prec_symbol == '>') {
            int output;
            if ((output = reduce())) {
                empty_stack(expr_stack);
                return output;
            }
        //shift prec
        } else if (prec_symbol == '<') {
            //generate code
            //push variables on expr_stack for future work with them
            if ((given_symbol == INT) || (given_symbol == DOUBLE) ||
                (given_symbol == STRING) || (given_symbol == ID)) {
                if (!(code_generate_stack_push(token))) {
                    return ERR_INTERNAL;
                }
            }
            if (!(insert_after_top_term(expr_stack, STOP, TYPE_UNDEFINED))) {
                empty_stack(expr_stack);
                return ERR_INTERNAL;
            }
            if ((given_symbol == INT) && (token->attribute.int_value == 0)) {
                is_zero = true;
            } else if ((given_symbol == DOUBLE) && (token->attribute.double_value == 0.0)) {
                is_zero = true;
            } else {
                is_zero = false;
            }
            if(!(push_stack(expr_stack, given_symbol, get_elem_type(given_symbol, token, data_stack), is_zero))) {
                empty_stack(expr_stack);
                return ERR_INTERNAL;
            }
            GET_TOKEN;
        //equal prec
        } else if (prec_symbol == '=') {
            if ((given_symbol == INT) && (token->attribute.int_value == 0)) {
                is_zero = true;
            } else if ((given_symbol == DOUBLE) && (token->attribute.double_value == 0.0)) {
                is_zero = true;
            } else {
                is_zero = false;
            }
            if(!(push_stack(expr_stack, given_symbol, get_elem_type(given_symbol, token, data_stack), is_zero))) {
                empty_stack(expr_stack);
                return ERR_INTERNAL;
            }
            GET_TOKEN;
        //error prec
        } else if (prec_symbol == '#') {
            if ((stack_term->elem == SIGN) && (given_symbol == SIGN)) {
                //successful end
                break;
            } else {
                empty_stack(expr_stack);
                return ERR_SYNTAX;
            }
        }
    }

    item_stack_t *final;
    if ((final = stack_top(expr_stack)) == NULL) {
        empty_stack(expr_stack);
        return ERR_INTERNAL;
    } else if (final->elem != EXPR) {
        empty_stack(expr_stack);
        return ERR_SYNTAX;
    }
    *expression_type = final->type;

    empty_stack(expr_stack);
    return OK;
}
