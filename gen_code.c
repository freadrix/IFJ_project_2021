/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Code generation
 * @author TODO
 */

#include <stdio.h>
#include <stdlib.h>

#include "gen_code.h"

string_struct generated_code;


bool builtin_reads() {

    return true;
}

bool builtin_readi() {

    return true;
}

bool builtin_readn() {

    return true;
}

bool builtin_write() {

    return true;
}

bool builtin_tointeger() {

    return true;
}

bool builtin_substr() {

    return true;
}

bool builtin_ord() {

    return true;
}

bool builtin_chr() {

    if(!(add_string_to_string(&generated_code, (
        "LABEL &chr\n"
        "PUSHFRAME\n"
        "DEFVAR LF@ret\n"
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
    if(!(add_string_to_string(&generated_code, (".IFJcode21\n")))) {
        return false;
    }
    //generate built-in functions
    if(!(builtin_reads())  || !(builtin_readi())  || !(builtin_readn())  || !(builtin_write()) ||
    !(builtin_tointeger()) || !(builtin_substr()) || !(builtin_ord())    || !(builtin_chr())) {
        return false;
    }
}

void code_write_out(FILE *out_file) {

    //write out code
    fputs(generated_code.string, out_file);
    //free string
    string_free(&generated_code);
    return true;
}
