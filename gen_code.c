/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Code generation
 * @author Aleksandr Verevkin (xverev00)
 */

#include <stdio.h>
#include <stdlib.h>

#include "gen_code.h"
#include "str.h"

string_struct generated_code;


//built-in reads function
//read string from user input
//return input in LF@%retval_1 in case of success, if input type is wrong, return nil
bool builtin_reads() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $reads\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval_1\n"
        "DEFVAR LF@%reads_type\n"
        //scan for string
        "READ LF@%retval_1 string\n"
        "TYPE LF@%reads_type LF@%retval_1\n"
        "JUMPIFNEQ $reads_nil_ret LF@%reads_type string@string\n"
        "JUMP $reads_ret\n"
        //return nil in case of wrong type
        "LABEL $reads_nil_ret\n"
        "MOVE LF@%retval_1 nil@nil\n"

        "LABEL $reads_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in readi function
//read integer from user input
//return input in LF@%retval_1 in case of success, if input type is wrong, return nil
bool builtin_readi() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $readi\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval_1\n"
        "DEFVAR LF@%readi_type\n"
        //scan for integer
        "READ LF@%retval_1 int\n"
        "TYPE LF@%readi_type LF@%retval_1\n"
        "JUMPIFNEQ $readi_nil_ret LF@%readi_type string@int\n"
        "JUMP $readi_ret\n"
        //return nil in case of wrong type
        "LABEL $readi_nil_ret\n"
        "MOVE LF@%retval_1 nil@nil\n"

        "LABEL $readi_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in readn function
//read float from user input
//return input in LF@%retval_1 in case of success, if input type is wrong, return nil
bool builtin_readn() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $readn\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval_1\n"
        "DEFVAR LF@%readn_type\n"
        //scan for float
        "READ LF@%retval_1 float\n"
        "TYPE LF@%readn_type LF@%retval_1\n"
        "JUMPIFNEQ $readn_nil_ret LF@%readn_type string@float\n"
        "JUMP $readn_ret\n"
        //return nil in case of wrong type
        "LABEL $readn_nil_ret\n"
        "MOVE LF@%retval_1 nil@nil\n"

        "LABEL $readn_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in write function
//write all passed as parameters terms
//parameters passed ON STACK
//count of terms is stored ON TOP OF THE STACK
//terms is stored AFTER TOP OF THE STACK
bool builtin_write() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $write\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%write_count\n"
        "POPS LF@%write_count\n"
        "DEFVAR LF@%write_str\n"

        "LABEL $write_loop\n"
        "POPS LF@%write_str\n"
        "WRITE LF@%write_str\n"
        "SUB LF@%write_count LF@%write_count int@1\n"
        "JUMPIFNEQ $write_loop LF@%write_count int@0\n"

        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in tointeger function
//convert float to integer
//take float in TF@%0 as parameter
//return integer in LF@%retval_1, nil if parameter was nil
//exit with error 5 if given parameter of wrong type
bool builtin_tointeger() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $tointeger\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval_1\n"
        //check if parameter is nil
        "JUMPIFEQ $toint_nil_ret nil@nil LF@%0\n"
        //check if parameter isn'float
        "TYPE LF@%retval_1 LF@%0\n"
        "JUMPIFEQ $toint_err_5 LF@%retval_1 string@float\n"
        "EXIT int@5\n"
        "LABEL $toint_err_5\n"
        //convert float to integer
        "FLOAT2INT LF@%retval_1 LF@%0\n"
        "JUMP $toint_ret\n"

        "LABEL $toint_nil_ret\n"
        "MOVE LF@%retval_1 nil@nil\n"

        "LABEL $toint_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in substr function
//found substring, of given string, trough given indexes
//return substring of given string in LF@%retval_1, empty string in case of wrong intervals
//exit with error 5 if given parameter of wrong type
//exit with error 8 if given nil in parameter
bool builtin_substr() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $substr\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval_1\n"
        //check of one of the parameters is nil
        "JUMPIFEQ $substr_err_8 nil@nil LF@%0\n"
        "JUMPIFEQ $substr_err_8 nil@nil LF@%1\n"
        "JUMPIFEQ $substr_err_8 nil@nil LF@%2\n"
        "JUMP $substr_err_8_end\n"
        "LABEL $substr_err_8\n"
        "EXIT int@8\n"
        "LABEL $substr_err_8_end\n"
        //check if given wrong type of parameter
        "DEFVAR LF@%type1\n"
        "TYPE LF@%type1 LF@%0\n"
        "DEFVAR LF@%type2\n"
        "TYPE LF@%type2 LF@%1\n"
        "DEFVAR LF@%type3\n"
        "TYPE LF@%type3 LF@%2\n"
        "JUMPIFNEQ $substr_err_5 string@string LF@%type1\n"
        "JUMPIFNEQ $substr_err_5 string@int LF@%type2\n"
        "JUMPIFNEQ $substr_err_5 string@int LF@%type3\n"
        "JUMP $substr_err_5_end\n"
        "LABEL $substr_err_5\n"
        "EXIT int@5\n"
        "LABEL $substr_err_5_end\n"
        //check if given indexes are in the interval of string
        //return empty string if something wrong
        "MOVE LF@%retval_1 string@\n"
        "DEFVAR LF@%len_check\n"
        "GT LF@%len_check LF@%1 LF@%2\n"
        "JUMPIFEQ $substr_ret bool@true LF@%len_check\n"

        "LT LF@%len_check LF@%1 int@1\n"
        "JUMPIFEQ $substr_ret bool@true LF@%len_check\n"

        "LT LF@%len_check LF@%2 int@1\n"
        "JUMPIFEQ $substr_ret bool@true LF@%len_check\n"

        "DEFVAR LF@%length\n"
        "STRLEN LF@%length LF@%0\n"

        "GT LF@%len_check LF@%1 LF@%length\n"
        "JUMPIFEQ $substr_ret bool@true LF@%len_check\n"

        "GT LF@%len_check LF@%2 LF@%length\n"
        "JUMPIFEQ $substr_ret bool@true LF@%len_check\n"

        "DEFVAR LF@%index\n"
        "SUB LF@%index LF@%1 int@1\n"
        "DEFVAR LF@%char\n"
        //concatenate each character trough the loop
        "LABEL $substr_loop\n"
        "GETCHAR LF@%char LF@%0 LF@%index\n"
        "CONCAT LF@%retval_1 LF@%retval_1 LF@%char\n"
        "ADD LF@%index LF@%index int@1\n"
        "JUMPIFNEQ $substr_loop LF@%index LF@%2\n"

        "LABEL $substr_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in chr function
//convert character of given string to his ascii value
//return integer in LF@%retval_1, nil if wrong interval
//exit with error 5 if given parameter of wrong type
//exit with error 8 if given nil in parameter
bool builtin_ord() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $ord\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval_1\n"
        //check of one of the parameters is nil
        "JUMPIFEQ $ord_err_8 nil@nil LF@%0\n"
        "JUMPIFEQ $ord_err_8 nil@nil LF@%1\n"
        "JUMP $ord_err_8_end\n"
        "LABEL $ord_err_8\n"
        "EXIT int@8\n"
        "LABEL $ord_err_8_end\n"
        //check if given wrong type of parameter
        "DEFVAR LF@%type1\n"
        "TYPE LF@%type1 LF@%0\n"
        "DEFVAR LF@%type2\n"
        "TYPE LF@%type2 LF@%1\n"
        "JUMPIFNEQ $ord_err_5 string@string LF@%type1\n"
        "JUMPIFNEQ $ord_err_5 string@int LF@%type2\n"
        "JUMP $ord_err_5_end\n"
        "LABEL $ord_err_5\n"
        "EXIT int@5\n"
        "LABEL $ord_err_5_end\n"
        //check for if given index is existing in the string
        "MOVE LF@%retval_1 nil@nil\n"
        "DEFVAR LF@%interval\n"
        "LT LF@%interval LF@%1 int@1\n"
        "JUMPIFEQ $ord_ret bool@true LF@%interval\n"

        "DEFVAR LF@%length\n"
        "STRLEN LF@%length LF@%0\n"
        "GT LF@%interval LF@%1 LF@%length\n"
        "JUMPIFEQ $ord_ret bool@true LF@%interval\n"
        //found character in the string and convert it into integer
        "SUB LF@%retval_1 LF@%1 int@1\n"
        "STRI2INT LF@%retval_1 LF@%0 LF@%retval_1\n"

        "LABEL $ord_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in chr function
//convert number to ascii character
//take integer in TF@%0 as parameter
//return character in LF@%retval_1, nil if wrong interval [0,255]
//exit with error 5 if given parameter of wrong type
//exit with error 8 if given nil in parameter
bool builtin_chr() {
    if(!(add_string_to_string(&generated_code, (
        "\nLABEL $chr\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval_1\n"
        //check for nil in parameter
        "JUMPIFNEQ $chr_err_8 nil@nil LF@%0\n"
        "EXIT int@8\n"
        "LABEL $chr_err_8\n"
        //check if given parameter isn't integer
        "TYPE LF@%retval_1 LF@%0\n"
        "JUMPIFEQ $chr_err_5 LF@%retval_1 string@int\n"
        "EXIT int@5\n"
        "LABEL $chr_err_5\n"
        //check if inerval of integer is right
        "DEFVAR LF@nil_range\n"
        "GT LF@nil_range LF@%0 int@255\n"
        "JUMPIFEQ $chr_nil_ret bool@true LF@nil_range\n"
        "LT LF@nil_range LF@%0 int@0\n"
        "JUMPIFEQ $chr_nil_ret bool@true LF@nil_range\n"
        //convert integer to character
        "INT2CHAR LF@%retval_1 LF@%0\n"
        "JUMP $chr_ret\n"

        "LABEL $chr_nil_ret\n"
        "MOVE LF@%retval_1 nil@nil\n"

        "LABEL $chr_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//use at the start of generating code
bool code_generator_init() {
    //initialize string
    if (!(string_init(&generated_code))) {
        return false;
    }
    //generate header
    if(!(add_string_to_string(&generated_code, (".IFJcode21\n"
                                                "DEFVAR GF@%gl_1\n"
                                                "DEFVAR GF@%gl_2\n"
                                                "DEFVAR GF@%gl_res\n"
                                                "JUMP $$main\n")))) {
        return false;
    }
    //generate built-in functions
    if(!(builtin_reads())  || !(builtin_readi())  || !(builtin_readn())  || !(builtin_write()) ||
    !(builtin_tointeger()) || !(builtin_substr()) || !(builtin_ord())    || !(builtin_chr())) {
        return false;
    }
    return true;
}

//use at the end of generating code
void code_write_out(FILE *out_file) {
    //write out code
    fputs(generated_code.string, out_file);
    //free string
    string_free(&generated_code);
}

//use in case of error
void free_generated_code_string() {
    string_free(&generated_code);
}

//use after code_generator_init
bool code_generate_main_start() {
    if(!(add_string_to_string(&generated_code, ("\nLABEL $$main\n"
                                                "CREATEFRAME\n"
                                                "PUSHFRAME\n")))) {
        return false;
    }
    return true;
}

//use at the end of code generation
bool code_generate_main_end() {
    if(!(add_string_to_string(&generated_code, ("POPFRAME\n"
                                                "CLEARS\n")))) {
        return false;
    }
    return true;
}

bool code_generate_function_call(char *f_name) {
    if(!(add_string_to_string(&generated_code, ("CALL $"))) ||
       !(add_string_to_string(&generated_code, (f_name))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}

bool code_generate_empty_variables_frame() {
    if(!(add_string_to_string(&generated_code, ("CREATEFRAME\n")))) {
        return false;
    }
    return true;    
}

//help function to find and generate value of given token
static bool code_generate_token_value(token_struct token) {
    string_struct term_value;
    //initialize string
    if (!(string_init(&term_value))) {
        return false;
    }
    char term[50];
    char ch;
    if (token.type == TOKEN_ID) {
        if (!(add_string_to_string(&generated_code, ("LF@"))) || 
            !(add_string_to_string(&generated_code, (token.attribute.string->string)))) {
                return false;
            }
    } else if (token.type == TOKEN_INT) {
        sprintf(term, "%d", token.attribute.int_value);
        if (!(add_string_to_string(&generated_code, ("int@"))) || 
            !(add_string_to_string(&generated_code, term))) {
                return false;
            }
    } else if (token.type == TOKEN_DOUBLE) {
        sprintf(term, "%a", token.attribute.double_value);
        if (!(add_string_to_string(&generated_code, ("float@"))) || 
            !(add_string_to_string(&generated_code, term))) {
                return false;
            }
    } else if (token.type == TOKEN_STRING) {
        for (int i = 0; (ch = (token.attribute.string->string)[i]) != '\0'; i++) {
            if (ch == '#' || ch == '\\' || ch < 33 || !(isprint(ch))) {
                sprintf(term, "%03d", ch);
                if (!(add_char_to_string(&term_value, ('\\'))) ||
                    !(add_string_to_string(&term_value, term))) {
                    return false;
                }
            } else {
                if (!(add_char_to_string(&term_value, ch))) {
                    return false;
                }
            }
        }
        if (!(add_string_to_string(&generated_code, ("string@"))) || 
            !(add_string_to_string(&generated_code, (term_value.string)))) {
                return false;
            }
    } else {
        string_free(&term_value);
        return false;
    }
    string_free(&term_value);
    return true;
}

bool code_generate_function_parameter(token_struct token, int param_num) {
    char index[10];
    sprintf(index, "%d", param_num);
    if(!(add_string_to_string(&generated_code, ("DEFVAR TF@%"))) || 
       !(add_string_to_string(&generated_code, (index))) ||
       !(add_string_to_string(&generated_code, ("\n"
                                                "MOVE TF@%"))) ||
       !(add_string_to_string(&generated_code, (index))) ||
       !(add_string_to_string(&generated_code, (" "))) ||
       !(code_generate_token_value(token)) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;    
}

bool code_generate_save_expression_result_on_retval(int retval_index) {
    char index_char[10];
    sprintf(index_char, "%d", retval_index);
    if(!(add_string_to_string(&generated_code, ("MOVE LF@%retval_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, (" GF@%gl_res\n")))) {
        return false;
    }
    return true;
}

bool code_generate_function_return(char *f_name) {
    if(!(add_string_to_string(&generated_code, ("JUMP $"))) ||
       !(add_string_to_string(&generated_code, (f_name))) ||
       !(add_string_to_string(&generated_code, ("_ret\n")))) {
        return false;
    }
    return true;  
}

bool code_generate_function_start(char *f_name) {
    if(!(add_string_to_string(&generated_code, ("LABLE $"))) ||
       !(add_string_to_string(&generated_code, (f_name))) ||
       !(add_string_to_string(&generated_code, ("\n"
                                                "PUSHFRAME\n")))) {
        return false;
    }
    return true;
}

bool code_generate_function_end(char *f_name) {
    if(!(add_string_to_string(&generated_code, ("LABLE $"))) ||
       !(add_string_to_string(&generated_code, (f_name))) ||
       !(add_string_to_string(&generated_code, ("_ret\n"
                                                "POPFRAME\n"
                                                "RETURN\n")))) {
        return false;
    }
    return true;
}

bool code_generate_retval_create(int retval_index) {
    char index_char[10];
    sprintf(index_char, "%d", retval_index);
    if(!(add_string_to_string(&generated_code, ("DEFVAR LF@%retval_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}

bool code_generate_retval_on_stack(int retval_index) {
    char index_char[10];
    sprintf(index_char, "%d", retval_index);
    if(!(add_string_to_string(&generated_code, ("PUSHS TF@%retval_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}

//help function for calling write(...), push strings on stack correctly
//rest of arguments must be STRINGS!!!
bool code_generate_write_function(int terms_count, ...) {
    //list of arguments
    va_list valist;
    va_start(valist, terms_count);
    //2d array for terms
    char *terms_arr[terms_count];
    //assign terms to array
    for (int i = 0; i > terms_count; i++) {
        terms_arr[i] = va_arg(valist, char*);
    }
    //generate push on stack from back of the array
    for (int i = terms_count - 1; i >= 0; i--) {
        if(!(add_string_to_string(&generated_code, ("PUSHS string@"))) ||
           !(add_string_to_string(&generated_code, terms_arr[i])) ||
           !(add_string_to_string(&generated_code, ("\n")))) {
            va_end(valist);
            return false;
        }
    }
    //put number of terms on top of the stack
    //and call write
    char count[10];
    sprintf(count, "%d", terms_count);
    if(!(add_string_to_string(&generated_code, ("PUSHS int@"))) ||
       !(add_string_to_string(&generated_code, (count))) ||
       !(add_string_to_string(&generated_code, ("\n"))) ||
       !(add_string_to_string(&generated_code, ("CALL $write\n")))) {
        va_end(valist);
        return false;
    }
    va_end(valist);
    return true;
}

bool code_generate_variable_create(char *var_name) {
    if(!(add_string_to_string(&generated_code, ("DEFVAR LF@"))) ||
       !(add_string_to_string(&generated_code, (var_name))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}

bool code_generate_variable_define_value(char *var_name, token_struct token) {
    if(!(add_string_to_string(&generated_code, ("MOVE LF@"))) ||
       !(add_string_to_string(&generated_code, (var_name))) ||
       !(add_string_to_string(&generated_code, (" "))) ||
       !(code_generate_token_value(token)) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}

bool code_generate_stack_push(token_struct token) {
    if(!(add_string_to_string(&generated_code, ("PUSHS "))) ||
       !(code_generate_token_value(token)) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}

// ======================== FLOAT<->INT CONVERTIONS START ======================== //

//expr handler
bool code_generate_stack_convert_float_first() {
    if(!(add_string_to_string(&generated_code, ("INT2FLOATS\n")))) {
        return false;
    }
    return true;
}

bool code_generate_stack_convert_float_second() {
    if(!(add_string_to_string(&generated_code, ("POPS GF@%gl_1\n"
                                                "INT2FLOATS\n"
                                                "PUSHS GF@%gl_1\n")))) {
        return false;
    }
    return true;    
}

bool code_generate_stack_convert_int_first() {
    if(!(add_string_to_string(&generated_code, ("FLOAT2INTS\n")))) {
        return false;
    }
    return true;    
}

bool code_generate_stack_convert_int_second() {
    if(!(add_string_to_string(&generated_code, ("POPS GF@%gl_1\n"
                                                "FLOAT2INTS\n"
                                                "PUSHS GF@%gl_1\n")))) {
        return false;
    }
    return true;        
}
// ======================== FLOAT<->INT CONVERTIONS END ======================== //

//expr handler
bool code_generate_operations(rules_enum r) {
    if (r == E_LT_E) {
        if(!(add_string_to_string(&generated_code, ("LTS\n")))) {
            return false;
        } 
    } else if (r == E_GT_E) {
        if(!(add_string_to_string(&generated_code, ("GTS\n")))) {
            return false;
        }
    } else if (r == E_GEQ_E) {
        if(!(add_string_to_string(&generated_code, (
            "POPS GF@%gl_1\n"
            "POPS GF@%gl_2\n"
            "PUSHS GF@%gl_2\n"
            "PUSHS GF@%gl_1\n"
            "GTS\n"
            "PUSHS GF@%gl_2\n"
            "PUSHS GF@%gl_1\n"
            "EQS\n"
            "ORS\n")))) {
            return false;
        } 
    } else if (r == E_LEQ_E) {
        if(!(add_string_to_string(&generated_code, (
            "POPS GF@%gl_1\n"
            "POPS GF@%gl_2\n"
            "PUSHS GF@%gl_2\n"
            "PUSHS GF@%gl_1\n"
            "LTS\n"
            "PUSHS GF@%gl_2\n"
            "PUSHS GF@%gl_1\n"
            "EQS\n"
            "ORS\n")))) {
            return false;
        } 
    } else if (r == E_EQ_E) {
        if(!(add_string_to_string(&generated_code, ("EQS\n")))) {
            return false;
        }
    } else if (r == E_NE_E) {
        if(!(add_string_to_string(&generated_code, ("EQS\n"
                                                    "NOTS\n")))) {
            return false;
        }
    } else if (r == E_MINUS_E) {
        if(!(add_string_to_string(&generated_code, ("SUBS\n")))) {
            return false;
        }        
    } else if (r == E_PLUS_E) {
        if(!(add_string_to_string(&generated_code, ("ADDS\n")))) {
            return false;
        }        
    } else if (r == E_MUL_E) {
        if(!(add_string_to_string(&generated_code, ("MULS\n")))) {
            return false;
        }        
    } else if (r == E_DIV_E) {
        if(!(add_string_to_string(&generated_code, ("DIVS\n")))) {
            return false;
        }        
    } else if (r == E_IDIV_E) {
        if(!(add_string_to_string(&generated_code, ("IDIVS\n")))) {
            return false;
        }
    } else if (r == E_CONCAT_E) {
        if(!(add_string_to_string(&generated_code, ("POPS GF@%gl_1\n"
                                                    "STRLEN GF@%gl_1 GF@%gl_1\n"
                                                    "PUSHS GF@%gl_1\n")))) {
        return false;
        }
    } else if (r = E_LEN) {
        if(!(add_string_to_string(&generated_code, ("POPS GF@%gl_2\n"
                                                    "POPS GF@%gl_1\n"
                                                    "CONCAT GF@%gl_1 GF@%gl_1 GF@%gl_2\n"
                                                    "PUSHS GF@%gl_1\n")))) {
        return false;
        }
    }
    return true;
}

//expr handler
bool code_generate_pop_stack_result() {
    if(!(add_string_to_string(&generated_code, ("POPS GF@%gl_res\n")))) {
        return false;
    }
    return true;
}

// ======================== IF ELSE START ======================== //
bool code_generate_if_start(int if_index) {
    char index_char[10];
    sprintf(index_char, "%d", if_index);
    if(!(add_string_to_string(&generated_code, ("TYPE GF@%gl_1 GF@%gl_res\n"))) ||
       !(add_string_to_string(&generated_code, ("JUMPIFEQ $IF_TYPE_START_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, (" GF@%gl_1 string@bool\n"))) ||
       !(add_string_to_string(&generated_code, ("JUMPIFEQ $IF_TYPE_START_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("MOVE GF@%gl_res bool@true\n"))) ||
       !(add_string_to_string(&generated_code, (" GF@%gl_1 string@nil\n"))) ||
       !(add_string_to_string(&generated_code, ("LABEL $IF_TYPE_START_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n"))) ||
       !(add_string_to_string(&generated_code, ("JUMPIFNEQ $ENDIF_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, (" GF@%gl_res bool@true\n")))) {
        return false;
    }
    return true;
}

bool code_generate_else(int if_index) {
    char index_char[10];
    sprintf(index_char, "%d", if_index);
    if(!(add_string_to_string(&generated_code, ("JUMP $ENDELSE_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n"
                                                "LABEL $ENDIF_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;    
}

bool code_generate_if_end(int if_index) {
    char index_char[10];
    sprintf(index_char, "%d", if_index);
    if(!(add_string_to_string(&generated_code, ("LABEL $ENDELSE_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}
// ======================== IF ELSE END ======================== //

// ======================== WHILE START ======================== //
bool code_generate_while_start(int while_index) {
    char index_char[10];
    sprintf(index_char, "%d", while_index);
    if(!(add_string_to_string(&generated_code, ("TYPE GF@%gl_1 GF@%gl_res\n"))) ||
       !(add_string_to_string(&generated_code, ("JUMPIFEQ $WHILE_TYPE_START_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, (" GF@%gl_1 string@bool\n"))) ||
       !(add_string_to_string(&generated_code, ("JUMPIFEQ $WHILE_TYPE_START_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("MOVE GF@%gl_res bool@true\n"))) ||
       !(add_string_to_string(&generated_code, (" GF@%gl_1 string@nil\n"))) ||
       !(add_string_to_string(&generated_code, ("LABEL $WHILE_TYPE_START_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n"))) ||
       !(add_string_to_string(&generated_code, ("LABEL $WHILESTART_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}

bool code_generate_while_end(int while_index) {
    char index_char[10];
    sprintf(index_char, "%d", while_index);
    if(!(add_string_to_string(&generated_code, ("JUMPIFEQ $WHILEEND_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, (" GF@%gl_res bool@true\n"))) ||
       !(add_string_to_string(&generated_code, ("JUMP $WHILESTART_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n"))) ||
       !(add_string_to_string(&generated_code, ("LABEL $WHILEEND_"))) ||
       !(add_string_to_string(&generated_code, (index_char))) ||
       !(add_string_to_string(&generated_code, ("\n")))) {
        return false;
    }
    return true;
}
// ======================== WHILE END ======================== //
