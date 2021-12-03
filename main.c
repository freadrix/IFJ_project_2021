/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Main, start file
 * @author TODO
 */


#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "str.h"


int main() {
    // // simple check of scanner
    // token_struct *token = malloc(sizeof(token_struct*));
    // while (token->type != EOF) {
    //     get_token(token);
    // }
    // free(token);

    int global_response = parser();
    if (global_response != 0)
        fprintf(stderr, "%d\n", global_response);
    return 0;
}
