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
//return input in case of success, if input type is wrong, return nil
bool builtin_reads() {
    if(!(add_string_to_string(&generated_code, (
        "LABEL $reads\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "DEFVAR LF@%reads_type\n"
        //scan for string
        "READ LF@%retval string\n"
        "TYPE LF@%reads_type LF@%retval\n"
        "JUMPIFNEQ $reads_nil_ret LF@%reads_type string@string\n"
        "JUMP $reads_ret\n"
        //return nil in case of wrong type
        "LABEL $reads_nil_ret\n"
        "MOVE LF@%retval nil@nil\n"

        "LABEL $reads_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in readi function
//read integer from user input
//return input in case of success, if input type is wrong, return nil
bool builtin_readi() {
    if(!(add_string_to_string(&generated_code, (
        "LABEL $readi\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "DEFVAR LF@%readi_type\n"
        //scan for integer
        "READ LF@%retval int\n"
        "TYPE LF@%readi_type LF@%retval\n"
        "JUMPIFNEQ $readi_nil_ret LF@%readi_type string@int\n"
        "JUMP $readi_ret\n"
        //return nil in case of wrong type
        "LABEL $readi_nil_ret\n"
        "MOVE LF@%retval nil@nil\n"

        "LABEL $readi_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in readn function
//read float from user input
//return input in case of success, if input type is wrong, return nil
bool builtin_readn() {
    if(!(add_string_to_string(&generated_code, (
        "LABEL $readn\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "DEFVAR LF@%readn_type\n"
        //scan for float
        "READ LF@%retval float\n"
        "TYPE LF@%readn_type LF@%retval\n"
        "JUMPIFNEQ $readn_nil_ret LF@%readn_type string@float\n"
        "JUMP $readn_ret\n"
        //return nil in case of wrong type
        "LABEL $readn_nil_ret\n"
        "MOVE LF@%retval nil@nil\n"

        "LABEL $readn_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

bool builtin_write() {

    return true;
}

//built-in tointeger function
//convert float to integer
//take float in TF@%0 as parameter
//return integer in LF@%retval, nil if parameter was nil
//exit with error 5 if given parameter of wrong type
bool builtin_tointeger() {
    if(!(add_string_to_string(&generated_code, (
        "LABEL $tointeger\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        //check if parameter is nil
        "JUMPIFEQ $toint_nil_ret nil@nil LF@%0\n"
        //check if parameter isn'float
        "TYPE LF@%retval LF@%0\n"
        "JUMPIFEQ $toint_err_5 LF@%retval string@float\n"
        "EXIT int@5\n"
        "LABEL $toint_err_5\n"
        //convert float to integer
        "FLOAT2INT LF@%retval LF@%0\n"
        "JUMP $toint_ret\n"

        "LABEL $toint_nil_ret\n"
        "MOVE LF@%retval nil@nil\n"

        "LABEL $toint_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

//built-in substr function
//found substring, of given string, trough given indexes
//return substring of given string, empty string in case of wrong intervals
//exit with error 5 if given parameter of wrong type
//exit with error 8 if given nil in parameter
bool builtin_substr() {
    if(!(add_string_to_string(&generated_code, (
        "LABEL $substr\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        //check of ine of the parameters is nil
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
        "MOVE LF@%retval string@\n"
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
        "CONCAT LF@%retval LF@%retval LF@%char\n"
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
//return integer in LF@%retval, nil if wrong interval
//exit with error 5 if given parameter of wrong type
//exit with error 8 if given nil in parameter
bool builtin_ord() {
    if(!(add_string_to_string(&generated_code, (
        "LABEL $ord\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        //check of ine of the parameters is nil
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
        "MOVE LF@%retval nil@nil\n"
        "DEFVAR LF@%interval\n"
        "LT LF@%interval LF@%1 int@1\n"
        "JUMPIFEQ $ord_ret bool@true LF@%interval\n"

        "DEFVAR LF@%length\n"
        "STRLEN LF@%length LF@%0\n"
        "GT LF@%interval LF@%1 LF@%length\n"
        "JUMPIFEQ $ord_ret bool@true LF@%interval\n"
        //found character in the string and convert it into integer
        "SUB LF@%retval LF@%1 int@1\n"
        "STRI2INT LF@%retval LF@%0 LF@%retval\n"

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
//return character in LF@%retval, nil if wrong interval [0,255]
//exit with error 5 if given parameter of wrong type
//exit with error 8 if given nil in parameter
bool builtin_chr() {
    if(!(add_string_to_string(&generated_code, (
        "LABEL $chr\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        //check for nil in parameter
        "JUMPIFNEQ $chr_err_8 nil@nil LF@%0\n"
        "EXIT int@8\n"
        "LABEL $chr_err_8\n"
        //check if given parameter isn't integer
        "TYPE LF@%retval LF@%0\n"
        "JUMPIFEQ $chr_err_5 LF@%retval string@int\n"
        "EXIT int@5\n"
        "LABEL $chr_err_5\n"
        //check if inerval of integer is right
        "DEFVAR LF@nil_range\n"
        "GT LF@nil_range LF@%0 int@255\n"
        "JUMPIFEQ $chr_nil_ret bool@true LF@nil_range\n"
        "LT LF@nil_range LF@%0 int@0\n"
        "JUMPIFEQ $chr_nil_ret bool@true LF@nil_range\n"
        //convert integer to character
        "INT2CHAR LF@%retval LF@%0\n"
        "JUMP $chr_ret\n"

        "LABEL $chr_nil_ret\n"
        "MOVE LF@%retval nil@nil\n"

        "LABEL $chr_ret\n"
        "POPFRAME\n"
        "RETURN\n")))) {
        return false;
    }
    return true;
}

bool code_generator_init() {
    //initialize string
    if (!(string_init(&generated_code))) {
        return false;
    }
    //generate header
    if(!(add_string_to_string(&generated_code, (".IFJcode21\n"
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

void code_write_out(FILE *out_file) {
    //write out code
    fputs(generated_code.string, out_file);
    //free string
    string_free(&generated_code);
}

bool code_generate_main_start() {
    if(!(add_string_to_string(&generated_code, ("LABEL $$main\n"
                                                "CREATEFRAME\n"
                                                "PUSHFRAME\n")))) {
        return false;
    }
    return true;
}

bool code_generate_main_end() {
    if(!(add_string_to_string(&generated_code, ("POPFRAME\n"
                                                "CLEARS\n")))) {
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
