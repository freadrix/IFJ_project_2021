#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "str.h"
#include "scanner.h"

FILE *source_code;

int get_token(token_struct *token)
{
    if (source_code == NULL) {
        return ERR_INTERNAL;
    }

    int state = STATE_START;
    char c;
    char string_number[2] = { 0 };

    string_struct *str;

    token->attribute.string = str;

    if (!string_init(str)) {
        return ERR_INTERNAL;
    }

    while(true)
    {
        c = getchar();

        switch(state)
        {
            case (STATE_START):
                if (isspace(c)) {
                    state = STATE_START;
                }
                else if (c == '\n') {
                    state = STATE_EOL;
                }
				else if (isalpha(c) || c == '_') {
                    c = tolower(c); // A-Z -> a-z
                    add_char_to_string(str, c);
                    state = STATE_ID_OR_KEYWORD;
                }
                else if (c == '"') {
                    state = STATE_STRING_START;
                }
                else if (isdigit(c)) {
                    if (c == '0') {
                        c = getchar();
                        if (c == '.') { // 0. => double
                            add_char_to_string(str, c);
                            state = STATE_NUMBER_POINT;
                        }
                        else
                        if (isdigit(c)) { // if we get 01, we ignore 0 and add only 1
                            add_char_to_string(str, c);
                            state = STATE_NUMBER;
                        }
                    }
                    else {
                        add_char_to_string(str, c);
                        state = STATE_NUMBER;
                    }
                    break;
                }
                else if (c == '-') {
                    state = STATE_MINUS;
                }
                else if (c == '<' ) {
                    state = STATE_LESS_THAN;
                }
                else if (c == '>') {
                    state = STATE_MORE_THAN;
                }
                else if (c == '=') {
                    state = STATE_EQUAL;
                }
                else if (c == '~') {
                    state = STATE_TILDE;
                }
                else if (c == '/') {
                    state = STATE_SLASH;
                }
                else if (c == '.') {
                    token->type = TOKEN_DOT;
                    string_free(str);
                    return OK;
                }
                else if (c == ':') {
                    token->type = TOKEN_DDOT;
                    string_free(str);
                    return OK;
                }
                else if (c == ',') {
                    token->type = TOKEN_COMMA;
                    string_free(str);
                    return OK;
                }
                else if (c == ']') {
                    token->type = TOKEN_BRACKET_SQUARE_R;
                    string_free(str);
                    return OK;
                }
                else if (c == '[') {
                    token->type = TOKEN_BRACKET_SQUARE_L;
                    string_free(str);
                    return OK;
                }
                else if (c == ')') {
                    token->type = TOKEN_BRACKET_ROUND_R;
                    string_free(str);
                    return OK;
                }
                else if (c == '(') {
                    token->type = TOKEN_BRACKET_ROUND_L;
                    string_free(str);
                    return OK;
                }
                else if (c == '}') {
                    token->type = TOKEN_BRACKET_CURLY_R;
                    string_free(str);
                    return OK;
                }
                else if (c == '{') {
                    token->type = TOKEN_BRACKET_CURLY_L;
                    string_free(str);
                    return OK;
                }
                else if (c == EOF) {
                    token->type = TOKEN_EOF;
                    string_free(str);
                    return OK;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_ID_OR_KEYWORD):
                if (c == '_' || isalpha(c)) {
                    c = tolower(c); // A-Z -> a-z
                    add_char_to_string(str, c);
                }
                else {
                    ungetc(c, source_code);
                    if (strcmp(str->string, "do") == 0) {
                        token->attribute.keyword = KEYWORD_DO;
                    }
                    else
                    if (strcmp(str->string, "else") == 0) {
                        token->attribute.keyword = KEYWORD_ELSE;
                    }
                    else
                    if (strcmp(str->string, "end") == 0) {
                        token->attribute.keyword = KEYWORD_END;
                    }
                    else
                    if (strcmp(str->string, "function") == 0) {
                        token->attribute.keyword = KEYWORD_FUNCTION;
                    }
                    else
                    if (strcmp(str->string, "global") == 0) {
                        token->attribute.keyword = KEYWORD_GLOBAL;
                    }
                    else
                    if (strcmp(str->string, "if") == 0) {
                        token->attribute.keyword = KEYWORD_IF;
                    }
                    else
                    if (strcmp(str->string, "local") == 0) {
                        token->attribute.keyword = KEYWORD_LOCAL;
                    }
                    else
                    if (strcmp(str->string, "nil") == 0) {
                        token->attribute.keyword = KEYWORD_NIL;
                    }
                    else
                    if (strcmp(str->string, "require") == 0) {
                        token->attribute.keyword = KEYWORD_REQUIRE;
                    }
                    else
                    if (strcmp(str->string, "return") == 0) {
                        token->attribute.keyword = KEYWORD_RETURN;
                    }
                    else
                    if (strcmp(str->string, "then") == 0) {
                        token->attribute.keyword = KEYWORD_THEN;
                    }
                    else
                    if (strcmp(str->string, "while") == 0) {
                        token->attribute.keyword = KEYWORD_WHILE;
                    }
                    else
                    if (strcmp(str->string, "integer") == 0) {
                        token->attribute.keyword = KEYWORD_INTEGER;
                    }
                    else
                    if (strcmp(str->string, "number") == 0) {
                        token->attribute.keyword = KEYWORD_NUMBER;
                    }
                    else
                    if (strcmp(str->string, "string") == 0) {
                        token->attribute.keyword = KEYWORD_STRING;
                    }
                    else {
                        token->type = TOKEN_ID;
                    }

                    if (token->type != TOKEN_ID) {
                        token->type = TOKEN_KEYWORD;
                        string_free(str);
                        return OK;
                    }

                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_STRING_START):
                if (c < 32) {
                    string_free(str);
                    return ERR_LEXER;
                }
                else
                if (c == '\\') {
                    state = STATE_STRING_ESCAPE;
                }
                else
                if (c == '"') {
                    token->type = TOKEN_STRING;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_STRING_ESCAPE):
                if (c == '\\') {
                    c = '\\';
                    add_char_to_string(str, c);
                    state = STATE_STRING_START;
                }
                else
                if (c == '"') {
                    c == '\"';
                    add_char_to_string(str, c);
                    state = STATE_STRING_START;
                }
                else
                if (c == 'n') {
                    c == '\n';
                    add_char_to_string(str, c);
                    state = STATE_STRING_START;
                }
                else
                if (c == 't') {
                    c == '\t';
                    add_char_to_string(str, c);
                    state = STATE_STRING_START;
                }
                else
                if (c == '0') {
                    string_number[0] = c;
                    state = STATE_STRING_ESCAPE_ZERO;
                }
                else
                if (c == '1') {
                    string_number[0] = c;
                    state = STATE_STRING_ESCAPE_ONE;
                }
                else
                if (c == '2') {
                    string_number[0] = c;
                    state = STATE_STRING_ESCAPE_TWO;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_ZERO):
                if (c == '0') {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_ZERO_ZERO;
                }
                else
                if (isdigit(c)) {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_OTHER;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_ONE):
                if (isdigit(c)) {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_OTHER;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_TWO):
                if (0 <= atoi(&c) <= 4) { // 0..4
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_OTHER;
                }
                else
                if (c == '5') {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_TWO_FIVE;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_ZERO_ZERO):
                if (isdigit(c)) {
                    string_number[2] = c;
                    int value = atoi(string_number);
                    c = value;
                    add_char_to_string(str, c);
                    state = STATE_STRING_START;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_TWO_FIVE):
                if (0 <= atoi(&c) <= 5) { // 0..5
                    string_number[2] = c;
                    int value = atoi(string_number);
                    c = value;
                    add_char_to_string(str, c);
                    state = STATE_STRING_START;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_OTHER):
                if (isdigit(c)) {
                    string_number[2] = c;
                    int value = atoi(string_number);
                    c = value;
                    add_char_to_string(str, c);
                    state = STATE_STRING_START;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_NUMBER):
                if (isdigit(c)) {
                    add_char_to_string(str, c);
                }
                else
                if (tolower(c) == 'e') {
                    add_char_to_string(str, c);
                    state = STATE_NUMBER_EXPONENT;
                }
                else
                if (c == '.') {
                    add_char_to_string(str, c);
                    state = STATE_NUMBER_POINT;
                }
                else {
                    ungetc(c, source_code);
                    int value = atoi(str->string);
                    token->attribute.int_value = value;
                    token->type = TOKEN_INT;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_NUMBER_EXPONENT):
                if (isdigit(c)) {
                    add_char_to_string(str, c);
                    state = STATE_NUMBER_EXPONENT_END;
                }
                else
                if (c == '+' || c == '-') {
                    add_char_to_string(str, c);
                    state = STATE_NUMBER_EXPONENT_SIGN;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_NUMBER_EXPONENT_SIGN):
                if (isdigit(c)) {
                    add_char_to_string(str, c);
                    state = STATE_NUMBER_EXPONENT_END;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_NUMBER_EXPONENT_END):
                if (isdigit(c)) {
                    add_char_to_string(str, c);
                }
                else {
                    ungetc(c, source_code);
                    int value = atof(str->string);
                    token->attribute.int_value = value;
                    token->type = TOKEN_DOUBLE;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_NUMBER_POINT):
                if (isdigit(c)) {
                    add_char_to_string(str, c);
                    state = STATE_NUMBER_DOUBLE;
                }
                else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_NUMBER_DOUBLE):
                if (isdigit(c)) {
                    add_char_to_string(str, c);
                }
                else
                if (tolower(c) == 'e') {
                    add_char_to_string(str, c);
                    state = STATE_NUMBER_EXPONENT;
                }
                else {
                    ungetc(c, source_code);
                    int value = atof(str->string);
                    token->attribute.int_value = value;
                    token->type = TOKEN_DOUBLE;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_MINUS):
                // TODOOOOOOOOOOOOOOOO
                break;
            case (STATE_MORE_THAN):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_LESS_OR_EQ;
                }
                else {
                    ungetc(c, source_code);
                    token->type = TOKEN_LESS;
                }
                break;
            case (STATE_LESS_THAN):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_LESS_OR_EQ;
                }
                else {
                    ungetc(c, source_code);
                    token->type = TOKEN_GREATER;
                }
                break;
            case (STATE_EQUAL):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_ASSIGN;
                }
                else {
                    ungetc(c, source_code);
                    token->type = TOKEN_EQUAL;
                }
                break;
            case (STATE_TILDE):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_NOT_EQUAL;
                }
                else { // TODO error: non-existent character ~
                    ungetc(c, source_code);
                }
                break;
            case (STATE_SLASH):
                c = getchar();
                if (c == '/') {
                    token->type = TOKEN_IDIV;
                }
                else {
                    ungetc(c, source_code);
                    token->type = TOKEN_DIV;
                }
                break;
        }
    }
    return 0;
}
