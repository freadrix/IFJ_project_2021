/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Main, start file
 * @author TODO
 */


//#include <stdio.h>
//#include <stdlib.h>
//
//#include "parser.h"
//
//
//int main() {
//
//    FILE *fp = stdin;
//    // TODO output
//    // TODO errors handling (analysis?)
//
//    return 0;   //TODO errors return
//}
#include "scanner.h"
#include "str.h"


int main() {
    //char c;
    // while ((c=getchar()) != EOF) {
    token_struct *token = malloc(sizeof(token_struct*));
    get_token(token);
    //}
    return 0;
}