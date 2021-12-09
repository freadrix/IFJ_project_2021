/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Main, start file
 * @author Anton Medvedev (xmedve04)
 */


#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "str.h"


int main() {
    int global_response = parser();
    if (global_response != 0)
        return global_response;
    return 0;
}
