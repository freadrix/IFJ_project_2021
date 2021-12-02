/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Lexical analysis
 * @author Ivan Tsiareshkin (xtsiar00)
 */

#include "scanner.h"

string_struct *working_string;

void define_working_str(string_struct *s)
{
	working_string = s;
}

int get_token(token_struct *token) {
    int state = STATE_START;
    int c;
    char string_number[3] = { 0 };
    char hex[3] = { 0 };

    token->attribute.string = working_string;
    string_struct string;
    string_struct *str = &string;

    if (!string_init(str)) {
        return ERR_INTERNAL;
    }

    while(1)
    {
        c = getchar();
        switch(state)
        {
            case (STATE_START):
                if (c == '\n') {
                    state = STATE_EOL;
                } else if (isspace(c)) {
                    state = STATE_START;
                } else if (isalpha(c) || c == '_') {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_ID_OR_KEYWORD;
                } else if (c == '"') {
                    state = STATE_STRING_START;
                } else if (isdigit(c)) {
                    if (c == '0') {
                        c = getchar();
                        if (c == '.') {
                            if (!(add_char_to_string(str, c))) {
                                string_free(str);
                                return ERR_INTERNAL;
                            }
                            state = STATE_NUMBER_POINT;
                        } else if (tolower(c) == 'e') {
                            if (!(add_char_to_string(str, c))) {
                                string_free(str);
                                return ERR_INTERNAL;
                            }
                            state = STATE_NUMBER_EXPONENT;
                        } else if (isdigit(c)) {
                            if (!(add_char_to_string(str, c))) {
                                string_free(str);
                                return ERR_INTERNAL;
                            }
                            state = STATE_NUMBER;
                        } else {
                            ungetc(c, stdin);
                            int value = atoi(str->string);
                            token->attribute.int_value = value;
                            token->type = TOKEN_INT;
                            string_free(str);
                            return OK;
                        }
                    } else {
                        if (!(add_char_to_string(str, c))) {
                            string_free(str);
                            return ERR_INTERNAL;
                        }
                        state = STATE_NUMBER;
                    }
                    break;
                } else if (c == '-') {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_MINUS;
                } else if (c == '/') {
                    state = STATE_SLASH;
                } else if (c == '<') {
                    state = STATE_LESS_THAN;
                } else if (c == '>') {
                    state = STATE_GREATER_THAN;
                } else if (c == '=') {
                    state = STATE_EQUAL;
                } else if (c == '~') {
                    state = STATE_TILDE;
                } else if (c == '.') {
                    state = STATE_DOT;
				} else if (c == '+') {
					token->type = TOKEN_PLUS;
					string_free(str);
					return OK;
				} else if (c == '*') {
					token->type = TOKEN_MUL;
					string_free(str);
					return OK;
                } else if (c == ':') {
                    token->type = TOKEN_DDOT;
                    string_free(str);
                    return OK;
                } else if (c == ',') {
                    token->type = TOKEN_COMMA;
                    string_free(str);
                    return OK;
                } else if (c == ']') {
                    token->type = TOKEN_BRACKET_SQUARE_R;
                    string_free(str);
                    return OK;
                } else if (c == '[') {
                    token->type = TOKEN_BRACKET_SQUARE_L;
                    string_free(str);
                    return OK;
                } else if (c == ')') {
                    token->type = TOKEN_BRACKET_ROUND_R;
                    string_free(str);
                    return OK;
                } else if (c == '(') {
                    token->type = TOKEN_BRACKET_ROUND_L;
                    string_free(str);
                    return OK;
                } else if (c == '#') {
                    token->type = TOKEN_UNARY_LENGTH;
                    string_free(str);
                    return OK;
                } else if (c == EOF) {
                    token->type = TOKEN_EOF;
                    string_free(str);
                    return OK;
                }else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_EOL):
                if (isspace(c)) {
                    break;
                }
                ungetc(c, stdin);
                token->type = TOKEN_EOL;
                string_free(str);
                return OK;
            case (STATE_ID_OR_KEYWORD):
                if (c == '_' || isalpha(c) || isdigit(c)) {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                } else {
                    ungetc(c, stdin);
                    if (strcmp(str->string, "do") == 0) {
                        token->attribute.keyword = KEYWORD_DO;
                    } else if (strcmp(str->string, "else") == 0) {
                        token->attribute.keyword = KEYWORD_ELSE;
                    } else if (strcmp(str->string, "end") == 0) {
                        token->attribute.keyword = KEYWORD_END;
                    } else if (strcmp(str->string, "function") == 0) {
                        token->attribute.keyword = KEYWORD_FUNCTION;
                    } else if (strcmp(str->string, "global") == 0) {
                        token->attribute.keyword = KEYWORD_GLOBAL;
                    } else if (strcmp(str->string, "if") == 0) {
                        token->attribute.keyword = KEYWORD_IF;
                    } else if (strcmp(str->string, "local") == 0) {
                        token->attribute.keyword = KEYWORD_LOCAL;
                    } else if (strcmp(str->string, "nil") == 0) {
                        token->attribute.keyword = KEYWORD_NIL;
                    } else if (strcmp(str->string, "require") == 0) {
                        token->attribute.keyword = KEYWORD_REQUIRE;
                    } else if (strcmp(str->string, "return") == 0) {
                        token->attribute.keyword = KEYWORD_RETURN;
                    } else if (strcmp(str->string, "then") == 0) {
                        token->attribute.keyword = KEYWORD_THEN;
                    } else if (strcmp(str->string, "while") == 0) {
                        token->attribute.keyword = KEYWORD_WHILE;
                    } else if (strcmp(str->string, "integer") == 0) {
                        token->attribute.keyword = KEYWORD_INTEGER;
                    } else if (strcmp(str->string, "number") == 0) {
                        token->attribute.keyword = KEYWORD_NUMBER;
                    } else if (strcmp(str->string, "string") == 0) {
                        token->attribute.keyword = KEYWORD_STRING;
                    } else {
                        token->type = TOKEN_ID;
                        if(!(string_copy(str, token->attribute.string))) {
                            string_free(str);
                            return ERR_INTERNAL;
                        }
                        string_free(str);
                        return OK;
                    }
                    token->type = TOKEN_KEYWORD;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_STRING_START):
                if ((c < 32 && c != 10) || c == EOF) {
                    string_free(str);
                    return ERR_LEXER;
                } else if (c == '\\') {
                    if (isdigit(c = getchar())) {
                        ungetc(c, stdin);
                    } else {
                        ungetc(c, stdin);
                    }
                    state = STATE_STRING_ESCAPE;
                } else if (c == '"') {
                    token->type = TOKEN_STRING;
                    if(!(string_copy(str, token->attribute.string))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    string_free(str);
                    return OK;
                } else {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                }
                break;
            case (STATE_STRING_ESCAPE):;
                if (c == '\\') {
                    if (!(add_char_to_string(str, '\\'))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;
                } else if (c == '"') {
                    if (!(add_char_to_string(str, '"'))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;
                } else if (c == 'n') {
                    if (!(add_char_to_string(str, '\n'))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;
                } else if (c == 't') {
                    if (!(add_char_to_string(str, '\t'))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;
                } else if (c == '0') {
                    string_number[0] = c;
                    state = STATE_STRING_ESCAPE_ZERO;
                } else if (c == '1') {
                    string_number[0] = c;
                    state = STATE_STRING_ESCAPE_ONE;
                } else if (c == '2') {
                    string_number[0] = c;
                    state = STATE_STRING_ESCAPE_TWO;
                } else if (tolower(c) == 'x') {
                    state = STATE_STRING_ESCAPE_HEX;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_ZERO):
                if (c == '0') {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_ZERO_ZERO;
                } else if (isdigit(c)) {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_OTHER;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_ONE):
                if (isdigit(c)) {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_OTHER;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_TWO):
                if (0 <= c && c <= 4) { // 0..4
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_OTHER;
                } else if (c == '5') {
                    string_number[1] = c;
                    state = STATE_STRING_ESCAPE_TWO_FIVE;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_ZERO_ZERO):
                if (isdigit(c)) {
                    string_number[2] = c;
                    int value = atoi(string_number);
                    c = value;
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_TWO_FIVE):
                if (0 <= c && c <= 5) { // 0..5
                    string_number[2] = c;
                    int value = atoi(string_number);
                    c = value;
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_OTHER):
                if (isdigit(c)) {
                    string_number[2] = c;
                    int value = atoi(string_number);
                    c = value;
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_STRING_ESCAPE_HEX):
                hex[0] = hex[1] = hex[2] = '\0';
                if (isxdigit(c)) {
                    hex[0] = (char) c;
                    c = getchar();
                    if(isxdigit(c)) {
                        hex[1] = (char) c;
                    }
                }
                if (strlen(hex) == 2) {
                    int value;
                    if (isdigit(hex[0])) {
                        value = 16 * (hex[0] - '0');
                    } else if( hex[0] >= 'A' && hex[0] <='F') {
                        value = 16 * (hex[0] - 'A' + 10);
                    } else {
                        value = 16 * (hex[0] - 'a' + 10);
                    }
                    if(isdigit(hex[1])) {
                        value += hex[1] - '0';
                    } else if( hex[1] >= 'A' && hex[1] <='F') {
                        value += hex[1] - 'A' + 10;
                    } else {
                        value += hex[1] - 'a' + 10;
                    }
                    hex[0] = hex[1] = hex[2] = '\0';
                    if (!(add_char_to_string(str, value))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_STRING_START;

                } else {
                    ungetc(c, stdin);
                    string_free(str);
                    return ERR_LEXER;

                }
                break;
            case (STATE_NUMBER):
                if (isdigit(c)) {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                } else if (tolower(c) == 'e') {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_NUMBER_EXPONENT;
                } else if (c == '.') {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_NUMBER_POINT;
                } else {
                    ungetc(c, stdin);
                    int value = atoi(str->string);
                    token->attribute.int_value = value;
                    token->type = TOKEN_INT;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_NUMBER_EXPONENT):
                if (isdigit(c)) {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_NUMBER_EXPONENT_END;
                } else if (c == '+' || c == '-') {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_NUMBER_EXPONENT_SIGN;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_NUMBER_EXPONENT_SIGN):
                if (isdigit(c)) {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_NUMBER_EXPONENT_END;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_NUMBER_EXPONENT_END):
                if (isdigit(c)) {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                } else {
                    ungetc(c, stdin);
                    double value = atof(str->string);
                    token->attribute.double_value = value;
                    token->type = TOKEN_DOUBLE;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_NUMBER_POINT):
                if (isdigit(c)) {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_NUMBER_DOUBLE;
                } else {
                    string_free(str);
                    return ERR_LEXER;
                }
                break;
            case (STATE_NUMBER_DOUBLE):
                if (isdigit(c)) {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                } else if (tolower(c) == 'e') {
                    if (!(add_char_to_string(str, c))) {
                        string_free(str);
                        return ERR_INTERNAL;
                    }
                    state = STATE_NUMBER_EXPONENT;
                } else {
                    ungetc(c, stdin);
                    double value = atof(str->string);
                    token->attribute.double_value = value;
                    token->type = TOKEN_DOUBLE;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_MINUS):
                if (c == '-') {
                    string_clear(str);
                    state = STATE_COMMENT_START;
                } else {
                    ungetc(c, stdin);
                    token->type = TOKEN_MINUS;
                    string_free(str);
                    return OK;
                }
                break;
            case (STATE_COMMENT_START):
                if (c == '\n' || c == EOF) {
                    ungetc(c, stdin);
                    state = STATE_START;
                } else if (c == '[') {
                    state = STATE_COMMENT_BLOCK_START;
                }
                break;
            case (STATE_COMMENT_BLOCK_START):
                if (c == '\n' || c == EOF) {
                    ungetc(c, stdin);
                    state = STATE_START;
                } else
                if (c == '[') {
                    state = STATE_COMMENT_BLOCK;
                } else {
                    state = STATE_COMMENT_START;
                }
                break;
            case (STATE_COMMENT_BLOCK):
                if (c == EOF) {
                    ungetc(c, stdin);
                    string_free(str);
                    return ERR_LEXER;
                } else if (c == ']') {
                    state = STATE_COMMENT_BLOCK_END;
                }
                break;
            case (STATE_COMMENT_BLOCK_END):
                if (c == ']') {
                    state = STATE_START;
                } else {
                    state = STATE_COMMENT_BLOCK;
                }
                break;
            case (STATE_GREATER_THAN):
                if (c == '=') {
                    token->type = TOKEN_GREATER_OR_EQ;
                } else {
                    ungetc(c, stdin);
                    token->type = TOKEN_GREATER;
                }
                string_free(str);
                return OK;
            case (STATE_LESS_THAN):
                if (c == '=') {
                    token->type = TOKEN_LESS_OR_EQ;
                } else {
                    ungetc(c, stdin);
                    token->type = TOKEN_LESS;
                }
                string_free(str);
                return OK;
            case (STATE_EQUAL):
                if (c == '=') {
                    token->type = TOKEN_ASSIGN;
                } else {
                    ungetc(c, stdin);
                    token->type = TOKEN_EQUAL;
                }
                string_free(str);
                return OK;
            case (STATE_TILDE):
                if (c == '=') {
                    token->type = TOKEN_NOT_EQUAL;
                } else {
                    ungetc(c, stdin);
                    string_free(str);
                    return ERR_LEXER;
                }
                string_free(str);
                return OK;
            case (STATE_SLASH):
                if (c == '/') {
                    token->type = TOKEN_IDIV;
                } else {
                    ungetc(c, stdin);
                    token->type = TOKEN_DIV;
                }
                string_free(str);
                return OK;
            case (STATE_DOT):
                if (c == '.') {
                    token->type = TOKEN_CONCAT;
                } else {
                    ungetc(c, stdin);
                    string_free(str);
                    return ERR_LEXER;
                }
                string_free(str);
                return OK;
        }
    }
}
