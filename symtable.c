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

void init_hashtab(table_t *tab) {
    //initialization of hash table
    if (tab != NULL) {
        int i = 0;
        while (i < MAX_HT_SIZE) {
            (*tab)[i++] = NULL;
        }
    }
}

void delete_all_hashtab(table_t *tab) {
    //fully clear hash table and free allocated memory
    if (tab != NULL) {
        int i = 0;
        while (i < MAX_HT_SIZE) {
            table_item_t *iter_item = (*tab)[i];
            while (iter_item != NULL) {
                table_item_t *temp_iter_item = iter_item->next;
                if ((iter_item->value.parameters != NULL)) {
                    string_free(iter_item->value.parameters);
                    free(iter_item->value.parameters);
                }
                free(iter_item->key);
                free(iter_item);
                iter_item = temp_iter_item;
            }
            (*tab)[i++] = NULL;
        }
    }
}

item_t *search_hashtab(table_t *tab, char *key) {
    //search in the hash table for specific item
    if (tab != NULL && key != NULL) {
        table_item_t *iter_item = (*tab)[get_hash(key)];
        while (iter_item != NULL) {
            if (!strcmp(iter_item->key, key)) {
                return &iter_item->value;
            }
        iter_item = iter_item->next;
        }
    }
    return NULL;
}

bool new_parameter_hashtab(item_t *item, value_type type) {
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

item_t *insert_hashtab(table_t *tab, char *key) {
    //insert new element in the hash table
    if (tab != NULL && key != NULL) {
        table_item_t *item = (*tab)[get_hash(key)], *previous_item = NULL;
        //check if item already existing in the table
        while (item != NULL) {
            if (!(strcmp(item->key, key))) {
                return NULL;
            }
            previous_item = item;
            item = item->next;
        }
        //allocate memory for the new item
        table_item_t *inserted = malloc(sizeof(table_item_t));
        if (inserted != NULL) {
            inserted->key = malloc((strlen(key) + 1) * sizeof(char));
            if (inserted->key != NULL) {
                inserted->value.parameters = malloc(sizeof(string_struct));
                if (inserted->value.parameters != NULL) {
                    if (string_init(inserted->value.parameters)) {
                        strcpy(inserted->key, key);
                        inserted->value.defined = false;
                        inserted->value.gl_var = false;
                        if (previous_item != NULL) {
                            previous_item->next = inserted;
                        } else {
                            (*tab)[get_hash(key)] = inserted;
                        }
                        return &inserted->value;
                    } else {
                        free(inserted->value.parameters);
                        free(inserted->key);
                        free(inserted);
                    }
                } else {
                    free(inserted->key);
                    free(inserted);
                }
            } else {
                free(inserted);
            }
        }
    }
    return NULL;
}

bool delete_single_hashtab(table_t *tab, char *key) {
    //found and delete, if founded, single element in the hash table
    if (tab != NULL && key != NULL) {
        table_item_t *item = (*tab)[get_hash(key)], *previous_item = NULL;
        while (item != NULL) {
            if (!(strcmp(item->key, key))) {
                if (item->value.parameters != NULL) {
                    string_free(item->value.parameters);
                    free(item->value.parameters);
                }
                if (previous_item != NULL) {
                    previous_item->next = item->next;
                } else {
                    (*tab)[get_hash(key)] = (*tab)[get_hash(key)]->next;
                }
                free(item->key);
                free(item);
                return true;
            }
            previous_item = item;
            item = item->next;
        }
    }
    return false;
}
