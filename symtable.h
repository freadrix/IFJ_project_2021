#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdbool.h>

#include "scanner.h"
#include "str.h"

#define MAX_HT_SIZE 20003  //TODO  //How many? For optimization. Must be prime number


/**
 * @enum Type of data
 */
typedef enum
{
    TYPE_STRING = TOKEN_STRING,
    TYPE_INTEGER = TOKEN_INT,
    TYPE_DOUBLE = TOKEN_DOUBLE,
} value_type;

/**
 * @struct Item structure
 */
typedef struct
{
    char *key;
    value_type type;
    bool defined;
    bool global;
    string_struct *parametrs;

} item_t;


/**
 * @struct Single item in the hash table
 */
typedef struct table_item
{
  char *key;                    // key identifier
  item_t value;                 // item value
  struct table_item *next;      // pointer on the next element
} table_item_t;

typedef table_item_t *table_t[MAX_HT_SIZE];

void init_hashtab(table_t *tab);

//TODO


#endif