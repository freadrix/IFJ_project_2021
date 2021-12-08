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
#define STATE_COMMENT_START 28        // \n = start(10), EOF = err, [ = block start(29), else state_comment(30)
#define STATE_COMMENT_BLOCK_START 29  // \n = start(10), EOF = err, [ = comm block(31), else state_comment(30)
#define STATE_COMMENT 30              // \n = start(10), else state_comment(30)
#define STATE_COMMENT_BLOCK 31        // ] = comm block end(32), EOF = err, else comm block(31)
#define STATE_COMMENT_BLOCK_END 32    // ] = start(10), EOF = err,  else comm block(31)

// Number
#define STATE_NUMBER 33
#define STATE_NUMBER_POINT 34
#define STATE_NUMBER_DOUBLE 35
#define STATE_NUMBER_EXPONENT 36
#define STATE_NUMBER_EXPONENT_SIGN 37
#define STATE_NUMBER_EXPONENT_END 38

#define STATE_EOF 39 // End of file
/* STATES END */

/**
* @enum Token types
*/
typedef enum {
    TOKEN_EMPTY,            // 0
    TOKEN_EOF,              // 1

    TOKEN_ID,               // 2
    TOKEN_KEYWORD,          // 3

    TOKEN_INT,              // 4
    TOKEN_DOUBLE,           // 5
    TOKEN_STRING,           // 6

    TOKEN_EQUAL,            // 7
    TOKEN_NOT_EQUAL,        // 8
    TOKEN_PLUS,             // 9
    TOKEN_MINUS,            // 10
    TOKEN_MUL,              // 11
    TOKEN_DIV,              // 12
    TOKEN_IDIV,             // 13
    TOKEN_CONCAT,           // 14
    TOKEN_UNARY_LENGTH,     // 15

    TOKEN_ASSIGN,           // 16
    TOKEN_GREATER,          // 17
    TOKEN_GREATER_OR_EQ,    // 18
    TOKEN_LESS,             // 19
    TOKEN_LESS_OR_EQ,       // 20

    TOKEN_DDOT,             // 21
    TOKEN_COMMA,            // 22
    TOKEN_BRACKET_SQUARE_R, // 23
    TOKEN_BRACKET_SQUARE_L, // 24
    TOKEN_BRACKET_ROUND_R,  // 25
    TOKEN_BRACKET_ROUND_L   // 26
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
