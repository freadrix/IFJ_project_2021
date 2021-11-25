/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Hash table operations header
 * @author Aleksandr Verevkin (xverev00)
 */

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdbool.h>

#include "scanner.h"
#include "str.h"

//max size of the table
#define MAX_HT_SIZE 2000  //TODO //How many? For optimization. Must be prime number
#define MAX_RETURN_TYPES 10 // max values which func return
#define MAX_PARAMETERS 10   // max parameters which func can have


/**
 * @enum Type of data
 */
typedef enum {
    TYPE_NULL,
    TYPE_BOOL,
    TYPE_INTEGER = TOKEN_INT,
    TYPE_DOUBLE = TOKEN_DOUBLE,
    TYPE_STRING = TOKEN_STRING,
    TYPE_UNDEFINED
}tab_item_data_type;

/**
 * @enum Type of id
 * */
typedef enum {
    VARIABLE,
    FUNCTION,
    UNDEFINED,
}tab_item_id_type;

/**
 * @union value of table element
 * */
typedef union t_value {
    int int_value;
    double double_value;
    char *string_value;
}tab_item_value;

/**
 * @struct data of table element
 * */
typedef struct table_item_data {
    char *identifier;
    tab_item_id_type type_id;
    tab_item_data_type type_data;
    tab_item_data_type type_return_values[MAX_RETURN_TYPES];
    tab_item_data_type type_parameter_values[MAX_PARAMETERS];
    bool defined;
    tab_item_value value;
}tab_item_data_t;

/**
 * @struct Single item in the hash table
 */
typedef struct table_item {
    char *key;
    tab_item_data_t data;
    struct table_item *next_item;
}tab_item_t;

/**
 * @typedef type of table, which is array of items
 * */
typedef table_item_t *table_t[MAX_HT_SIZE];

/**
 * @brief Hash table initialization
 * @param tab Pointer on table
 */
void init_hashtable(table_t *tab);

/**
 * @brief Clear all symbols from hash table and free allocated memory
 * @param tab Pointer on table
 */
void delete_all_hashtable(table_t *tab);

/**
 * @brief Search item with given key
 * @param tab Pointer on table
 * @param key Key of needed item
 * @return Pointer on founded item in case of success, else NULL
 */
tab_item_t *search_hashtable(table_t *tab, char *key);

/**
 * @brief Add new parameter(ids) to the function
 * @param tab Pointer on table
 * @param type Value type of the new parameter
 * @return true in case of success, else false
 */
bool new_parameter_hashtable(table_item_t *item, value_type type);

/**
 * @brief Insert new element in the hash table
 * @param tab Pointer on table
 * @param key Key of inserted item
 * @return Pointer on created item in case of success, else NULL
 */
void insert_element_hashtable(table_t *tab, char *key);

/**
 * @brief Delete element from the hash table
 * @param tab Pointer on table
 * @param key Key of deleted item
 * @return true in case of success, else false
 */
bool delete_single_hashtable(table_t *tab, char *key);

#endif
