/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Lexical analysis header
 * @author Ivan Tsiareshkin (xtsiar00)
 */

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdio.h>
#include <ctype.h>

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
#define STATE_STRING_ESCAPE_HEX 20

#define STATE_DOT 21          // .
#define STATE_LESS_THAN 22    // <
#define STATE_GREATER_THAN 23 // >
#define STATE_EQUAL 24        // =
#define STATE_TILDE 25        // ~
#define STATE_MINUS 26        // -
#define STATE_SLASH 27        // /

// Comments
#define STATE_COMMENT_START 28        // ∑-EOL, EOF, [              (if [ go to 27)
#define STATE_COMMENT_BLOCK_START 29  // go back to state 26 if ∑-[
#define STATE_COMMENT_BLOCK 30        // ∑-]                        (if ] go to 31)
#define STATE_COMMENT_BLOCK_END 31    // go back to state 28 if ∑-]

// Number
#define STATE_NUMBER 32
#define STATE_NUMBER_POINT 33
#define STATE_NUMBER_DOUBLE 34
#define STATE_NUMBER_EXPONENT 35
#define STATE_NUMBER_EXPONENT_SIGN 36
#define STATE_NUMBER_EXPONENT_END 37

#define STATE_EOL 38 // End of line
#define STATE_EOF 39 // End of file
/* STATES END */

/**
* @enum Token types
*/
typedef enum {
    TOKEN_EMPTY,            // 0
    TOKEN_EOL,              // 1
    TOKEN_EOF,              // 2

    TOKEN_ID,               // 3
    TOKEN_KEYWORD,          // 4

    TOKEN_INT,              // 5
    TOKEN_DOUBLE,           // 6
    TOKEN_STRING,           // 7

    TOKEN_EQUAL,            // 8
    TOKEN_NOT_EQUAL,        // 9
    TOKEN_PLUS,             // 10
    TOKEN_MINUS,            // 11
    TOKEN_MUL,              // 12
    TOKEN_DIV,              // 13
    TOKEN_IDIV,             // 14
    TOKEN_CONCAT,           // 15
    TOKEN_UNARY_LENGTH,     // 16

    TOKEN_ASSIGN,           // 17
    TOKEN_GREATER,          // 18
    TOKEN_GREATER_OR_EQ,    // 19
    TOKEN_LESS,             // 20
    TOKEN_LESS_OR_EQ,       // 21

    TOKEN_DDOT,             // 22
    TOKEN_COMMA,            // 23
    TOKEN_BRACKET_SQUARE_R, // 24
    TOKEN_BRACKET_SQUARE_L, // 25
    TOKEN_BRACKET_ROUND_R,  // 26
    TOKEN_BRACKET_ROUND_L   // 27
} token_type;

/**
* @enum Keywords
*/
typedef enum {
    KEYWORD_DO,         // 0
    KEYWORD_ELSE,       // 1
    KEYWORD_END,        // 2
    KEYWORD_FUNCTION,   // 3
    KEYWORD_GLOBAL,     // 4
    KEYWORD_IF,         // 5
    KEYWORD_LOCAL,      // 6
    KEYWORD_NIL,        // 7
    KEYWORD_REQUIRE,    // 8
    KEYWORD_RETURN,     // 9
    KEYWORD_THEN,       // 10
    KEYWORD_WHILE,      // 11
    KEYWORD_INTEGER,    // 12
    KEYWORD_NUMBER,     // 13
    KEYWORD_STRING      // 14
} keyword_type;

/**
* @struct Token possible attributes
*/
typedef union {

    int int_value;
    double double_value;
    string_struct *string;
    keyword_type keyword;
} token_attribute;

/**
* @struct Token representation
*/
typedef struct {

    token_type type;
    token_attribute attribute;
} token_struct;

/**
* @brief function reads the input source file character by character and makes tokens by character value
* @param token Pointer to work with the token structure
*/
int get_token(token_struct *token);

/**
* @brief function defines a string in the scanner to work in the parser
* @param str String to define in scanner
*/
void define_working_str(string_struct *str);

#endif
