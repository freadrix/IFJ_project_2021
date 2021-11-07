/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Grammar header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

/**
 * @enum Available symbols
 */
typedef enum {

    LT,         // <
    GT,         // >
    GEQ,        // >=
    LEQ,        // <=
    EQ,         // ==
    NE,         // ~=
    MINUS,      // -
    PLUS,       // +
    MUL,        // *
    DIV,        // /
    IDIV,       // //

    LBR,        // (
    RBR,        // )
    ID,         // identifier
    INT,
    DOUBLE,
    STRING,
    
    EXP,        // expression
    SIGN,       // $
} elem_enum;

/**
 * @enum Rules
 */
typedef enum {

    E_LT_E,         // E < E
    E_GT_E,         // E > E
    E_GEQ_E,        // E >= E
    E_LEQ_E,        // E <= E
    E_EQ_E,         // E == E
    E_NE_E,         // E ~= E
    E_MINUS_E,      // E - E
    E_PLUS_E,       // E + E
    E_MUL_E,        // E * E
    E_DIV_E,        // E / E
    E_IDIV_E,       // E // E

    ID_RULE,        // id
    BR_E_BR,        // (E)
    NO_RULE
} rules_enum;


#endif
