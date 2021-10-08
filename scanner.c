#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include "str.h"
#include "scanner.h"

FILE *source_code;

int get_token(token_struct *token)
{
    int state = STATE_START;
    char c;

    while(state != STATE_EOF)
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
                    state = STATE_ID_OR_KEYWORD;
                }
                else if (c == '"') {
                    state = STATE_STRING_START;
                }
                else if (isdigit(c)) {
                    state = STATE_NUMBER;
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
                }
                else if (c == ':') {
                    token->type = TOKEN_DDOT;
                }
                else if (c == ',') {
                    token->type = TOKEN_COMMA;
                }
                else if (c == ']') {
                    token->type = TOKEN_BRACKET_SQUARE_R;
                }
                else if (c == '[') {
                    token->type = TOKEN_BRACKET_SQUARE_L;
                }
                else if (c == ')') {
                    token->type = TOKEN_BRACKET_ROUND_R;
                }
                else if (c == '(') {
                    token->type = TOKEN_BRACKET_ROUND_L;
                }
                else if (c == '}') {
                    token->type = TOKEN_BRACKET_CURLY_R;
                }
                else if (c == '{') {
                    token->type = TOKEN_BRACKET_CURLY_L;
                }
                else if (c == EOF) {
                    token->type = TOKEN_EOF;
                }
                break;
            case (STATE_ID_OR_KEYWORD):
                // TODO
                break;
            case (STATE_STRING_START):
                // TODO
                break;
            case (STATE_NUMBER):
                // TODO
                break;
            case (STATE_MINUS):
                // TODO
                break;
            case (STATE_MORE_THAN):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_LESS_OR_EQ;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_LESS;
                }
                break;
            case (STATE_LESS_THAN):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_LESS_OR_EQ;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_GREATER;
                }
                break;
            case (STATE_EQUAL):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_ASSIGN;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_EQUAL;
                }
                break;
            case (STATE_TILDE):
                c = getchar();
                if (c == '=') {
                    token->type = TOKEN_NOT_EQUAL;
                }
                else { // TODO error: non-existent character ~
                    ungetc(c, stdin);
                }
                break;
            case (STATE_SLASH):
                c = getchar();
                if (c == '/'){
                    token->type = TOKEN_IDIV;
                }
                else {
                    ungetc(c, stdin);
                    token->type = TOKEN_DIV;
                }
                break;
        }
    }
    return 0;
}
