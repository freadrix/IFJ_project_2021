/**
 * Implementace překladače imperativního jazyka IFJ21.
 * 
 * @brief Hash table operations
 * @author Aleksandr Verevkin (xverev00)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"

// Don't understand yet
static unsigned get_hash(const char *str) {
    //generator of hash
    unsigned h = 0;
    const unsigned char *us;
    us = (unsigned const char *) str;
    while((*us) != '\0') {
        h = 65599 * h + (*us);
        us++;
    }
    return (h % MAX_HT_SIZE);
}

void init_hashtable(table_t *tab) {
    //initialization of hash table
    for (int i = 0; i < MAX_HT_SIZE; ++i) {
        (*tab)[i] = NULL;
    }
}

void delete_all_hashtable(table_t *tab) {
    //fully clear hash table and free allocated memory
    if (tab != NULL) {
        int i = 0;
        while (i < MAX_HT_SIZE) {
            table_item_t *iter_item = (*tab)[i];
            while (iter_item != NULL) {
                table_item_t *temp_iter_item = iter_item->next;
//                if ((iter_item->value.parameters != NULL)) {
//                    string_free(iter_item->value.parameters);
//                    free(iter_item->value.parameters);
//                }
//                free(iter_item->key);
                free(iter_item);
                iter_item = temp_iter_item;
            }
            (*tab)[i++] = NULL;
        }
    }
}

tab_item_t *search_hashtable(table_t *tab, char *key) {
    //search in the hash table for specific item
    if (tab != NULL && key != NULL) {
        tab_item_t *iter_item = (*tab)[get_hash(key)];
        while (iter_item != NULL) {
            if (!strcmp(iter_item->key, key)) {
                return iter_item;
            }
        iter_item = iter_item->next;
        }
    }
    return NULL;
}

bool new_parameter_hashtable(table_item_t *item, value_type type) {
    //insert new parameter(int(i), double(d), str(s)) for function
    if (item != NULL) {
        if (type == TYPE_INTEGER) {
            if (!(add_char_to_string(item->parameters, 'i'))) {
                return false;
            }
        }
        else if (type == TYPE_DOUBLE) {
            if (!(add_char_to_string(item->parameters, 'd'))) {
                return false;
            }
        }
        else if (type == TYPE_STRING) {
            if (!(add_char_to_string(item->parameters, 's'))) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

tab_item_t *insert_element_hashtable(table_t *tab, char *key) {
    //insert new element in the hash table
    if (tab != NULL && key != NULL) {
        // check if item already exist
        tab_item_t *item = search_hashtable(tab, key);
        if (item != NULL) return NULL;
        //allocate memory for the new item

        tab_item_t *inserted = (tab_item_t *) malloc(sizeof(table_item_t));
        if (inserted == NULL) return;
        if (!(inserted->key = (char *)malloc((strlen(key) + 1) * sizeof(char)))) {
            free (inserted);
            return; NULL
        }
        if (!(inserted->data = (tab_item_data_t) malloc(sizeof (tab_item_data_t)))) {
            free (inserted->key);
            free (inserted);
            return NULL
        }
        strcpy(inserted->key, key);
        inserted->data.identifier = inserted->key;
        inserted->data.defined = false;
        inserted->data.type_id = UNDEFINED;
        inserted->data.type_data = TYPE_UNDEFINED;
        inserted->next = (*tab)[get_hash(key)];
        (*tab)[get_hash(key)] = inserted;
        return inserted;
    }
}

bool delete_single_hashtable(table_t *tab, char *key) {
    //found and delete, if founded, single element in the hash table
    if (tab != NULL && key != NULL) {
        table_item_t *item = (*tab)[get_hash(key)], *previous_item = NULL;
        while (item != NULL) {
            if (!(strcmp(item->key, key))) {
//                if (item->value.parameters != NULL) {
//                    string_free(item->value.parameters);
//                    free(item->value.parameters);
//                }
                if (previous_item != NULL) {
                    previous_item->next = item->next;
                } else {
                    (*tab)[get_hash(key)] = (*tab)[get_hash(key)]->next;
                }
//                free(item->key);
                free(item);
                return true;
            }
            previous_item = item;
            item = item->next;
        }
    }
    return false;
}
