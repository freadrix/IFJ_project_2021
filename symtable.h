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

// max size of the table
#define MAX_HT_SIZE 6133   //TODO //How many? For optimization. Must be prime number
#define MAX_RETURNS 10      // max values which func return
#define MAX_PARAMETERS 10   // max parameters which func can have


// structs
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
 * @struct parameter types
 * */
typedef struct table_item_parameters {
    tab_item_data_type type_parameters[MAX_PARAMETERS];
    int count_parameters;
}tab_item_parameters;

/**
 * @struct return types
 * */
typedef struct table_item_returns {
    tab_item_data_type type_returns[MAX_RETURNS];
    int count_returns;
}tab_item_returns;

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
    tab_item_id_type item_id_type;
    tab_item_data_type item_data_type;
    tab_item_returns item_returns;
    tab_item_parameters item_parameters;
    bool defined;
    tab_item_value item_value;   // mozna nepotrebujeme
}tab_item_data_t;

/**
 * @struct Single item in the hash table
 */
typedef struct table_item {
    char *key;
    tab_item_data_t *data;
    struct table_item *next_item;
}tab_item_t;


// functions
/**
 * @typedef type of table, which is array of items
 * */
typedef tab_item_t *table_t[MAX_HT_SIZE];

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
 * @brief Insert new element in the hash table
 * @param tab Pointer on table
 * @param key Key of inserted item
 * @return Pointer on created item in case of success, else NULL
 */
tab_item_t *insert_element_hashtable(table_t *tab, char *key);

/**
 * @brief Delete element from the hash table
 * @param tab Pointer on table
 * @param key Key of deleted item
 * @return true in case of success, else false
 */
bool delete_single_hashtable(table_t *tab, char *key);

/**
 * @brief function insert type in array of parameter types
 * @param item pointer on item of table
 * @param type type of parameter value
 * @return true in case of success, else false
 * */
bool insert_parameter_item(tab_item_t *item, tab_item_data_type type);

/**
 * @brief function insert type in array of return types
 * @param item pointer on item of table
 * @param type type of return value
 * @return true in case of success, else false
 * */
bool insert_return_item(tab_item_t *item, tab_item_data_type type);

#endif
