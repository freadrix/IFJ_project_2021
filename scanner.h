#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "error.h"
#include "str.h"

/* STATES */
#define STATE_START 10 // Start

#define STATE_ID_OR_KEYWORD 11 /*  Values: _, a-z, A-Z  */

// String
#define STATE_STRING_START 12            /*  Value: ASCII <32,255> without ", /  */
#define STATE_STRING_ESCAPE 13           /*  Value: /    */
#define STATE_STRING_ESCAPE_ZERO 14      /*  Value: /0   */
#define STATE_STRING_ESCAPE_ZERO_ZERO 15 /*  Value: /00  */
#define STATE_STRING_ESCAPE_ONE 16       /*  Value: /1   */
#define STATE_STRING_ESCAPE_TWO 17       /*  Value: /2   */
#define STATE_STRING_ESCAPE_TWO_FIVE 18  /*  Value: /25  */
#define STATE_STRING_ESCAPE_OTHER 19     /*  Value: from 001 to 249  */

#define STATE_LESS_THAN 20 // <
#define STATE_MORE_THAN 21 // >
#define STATE_EQUAL 22     // =
#define STATE_TILDE 23     // ~
#define STATE_MINUS 24     // -
#define STATE_SLASH 25     // /

// Comments
#define STATE_COMMENT_START 26        // ∑-EOL, EOF, [              (if [ go to 27)
#define STATE_COMMENT_BLOCK_START 27   // go back to state 26 if ∑-[
#define STATE_COMMENT_BLOCK 28        // ∑-]                        (if ] go to )
#define STATE_COMMENT_BLOCK_END 29     // go back to state 28 if ∑-]

// Number
#define STATE_NUMBER 30
#define STATE_NUMBER_POINT 31
#define STATE_NUMBER_DOUBLE 32
#define STATE_NUMBER_EXPONENT 33
#define STATE_NUMBER_EXPONENT_SIGN 34
#define STATE_NUMBER_EXPONENT_END 35

#define STATE_EOL 36 // End of line
#define STATE_EOF 37 // End of file
/* STATES END */

typedef enum {

    TOKEN_EOL,
    TOKEN_EOF,

    TOKEN_ID,
    TOKEN_KEYWORD,

    TOKEN_INT,
    TOKEN_DOUBLE,
    TOKEN_STRING,

    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_IDIV,

    TOKEN_ASSIGN,
    TOKEN_GREATER,
    TOKEN_GREATER_OR_EQ,
    TOKEN_LESS,
    TOKEN_LESS_OR_EQ,

    TOKEN_DOT,
    TOKEN_DDOT,
    TOKEN_COMMA,
    TOKEN_BRACKET_SQUARE_R,
    TOKEN_BRACKET_SQUARE_L,
    TOKEN_BRACKET_ROUND_R,
    TOKEN_BRACKET_ROUND_L,
    TOKEN_BRACKET_CURLY_R,
    TOKEN_BRACKET_CURLY_L,
} token_type;

typedef enum {

    KEYWORD_DO,
    KEYWORD_ELSE,
    KEYWORD_END,
    KEYWORD_FUNCTION,
    KEYWORD_GLOBAL,
    KEYWORD_IF,
    KEYWORD_LOCAL,
    KEYWORD_NIL,
    KEYWORD_REQUIRE,
    KEYWORD_RETURN,
    KEYWORD_THEN,
    KEYWORD_WHILE,
    KEYWORD_INTEGER,
    KEYWORD_NUMBER,
    KEYWORD_STRING,
} keyword_type;

typedef union {

    int int_value;
    double double_value;
    string_struct *string;
    keyword_type keyword;
} token_attribute;

typedef struct {
    
    token_type type;
    token_attribute attribute;
} token_struct;

int get_token(token_struct *token);

#endif
