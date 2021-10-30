/**
 * @brief Hash table operations header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdbool.h>

#include "scanner.h"
#include "str.h"

#define MAX_HT_SIZE 20003  //TODO //How many? For optimization. Must be prime number


/**
 * @enum Type of data
 */
typedef enum {

    TYPE_INTEGER = TOKEN_INT,
    TYPE_DOUBLE = TOKEN_DOUBLE,
    TYPE_STRING = TOKEN_STRING,
} value_type;

/**
 * @struct Item structure
 */
typedef struct {

    char *key;                  // item identifier
    value_type type;            // type of item value
    bool defined;               // existing of function
    bool gl_var;
    string_struct *parametrs;   // parametrs of function (i,d,s)
} item_t;


/**
 * @struct Single item in the hash table
 */
typedef struct table_item {

  char *key;                    // key identifier
  item_t value;                 // item value
  struct table_item *next;      // pointer on the next element
} table_item_t;

typedef table_item_t *table_t[MAX_HT_SIZE];

/**
 * @brief Hash table initialization
 * @param tab Pointer on table
 */
void init_hashtab(table_t *tab);

/**
 * @brief Clear all symbols from hash table and free allocated memory
 * @param tab Pointer on table
 */
void delete_all_hashtab(table_t *tab);

/**
 * @brief Hash table initialization
 * @param tab Pointer on table
 * @param key Key of needed item
 * @return Pointer on founded item in case of success, else NULL
 */
item_t *search_hashtab(table_t *tab, char *key);

/**
 * @brief Hash table initialization
 * @param tab Pointer on table
 * @param type Value type of the new parametr
 * @return true in case of success, else false
 */
bool new_parametr_hashtab(item_t *item, value_type type);

/**
 * @brief Hash table initialization
 * @param tab Pointer on table
 * @param key Key of inserted item
 * @return Pointer on created item in case of success, else NULL
 */
item_t *insert_hashtab(table_t *tab, char *key);

/**
 * @brief Hash table initialization
 * @param tab Pointer on table
 * @param key Key of deleted item
 * @return true in case of success, else false
 */
bool delete_single_hashtab(table_t *tab, char *key);

#endif