/**
 * Implementace překladače imperativního jazyka IFJ21.
 *
 * @brief Hash table operations
 * @authors Aleksandr Verevkin (xverev00) Anton Medvedev (xmedve04)
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
// !strcmp(iter_item->key, key)
tab_item_t *search_hashtable(table_t *tab, char *key) {
    //search in the hash table for specific item
    if (tab != NULL && key != NULL) {
        tab_item_t *iter_item = (*tab)[get_hash(key)];
        while (iter_item != NULL) {
            if (!strcmp(iter_item->key, key)) {
                return iter_item;
            }
            iter_item = iter_item->next_item;
        }
    }
    return NULL;
}

tab_item_t *insert_element_hashtable(table_t *tab, char *key) {
    //insert new element in the hash table
    if (tab != NULL && key != NULL) {
        // check if item already exist
        tab_item_t *item = search_hashtable(tab, key);
        if (item != NULL) return NULL;
        //allocate memory for the new item
        tab_item_t *inserted = (tab_item_t *) malloc(sizeof(tab_item_t));
        if (inserted == NULL) return NULL;
        if (!(inserted->key = (char *)malloc((strlen(key) + 1) * sizeof(char)))) {
            free(inserted);
            return NULL;
        }
        if (!(inserted->data = (tab_item_data_t *) malloc(sizeof(tab_item_data_t)))) { // error: conversion to non-scalar type requested
            free(inserted->key);
            free(inserted);
            return NULL;
        }
        if (!(inserted->data->identifier = (char *)malloc((strlen(key) + 1) * sizeof(char)))) {
            free(inserted->data);
            free(inserted->key);
            free(inserted);
            return NULL;
        }
        strcpy(inserted->key, key);
        strcpy(inserted->data->identifier, key);
        inserted->data->defined = false;
        inserted->data->item_id_type = UNDEFINED;
        inserted->data->item_data_type = TYPE_UNDEFINED;
        inserted->data->item_returns.count_returns = 0;
        inserted->data->item_parameters.count_parameters = 0;
        inserted->next_item = (*tab)[get_hash(key)];
        (*tab)[get_hash(key)] = inserted;
        return inserted;
    }
    return NULL;
}

bool delete_single_hashtable(table_t *tab, char *key) {
    //found and delete, if founded, single element in the hash table
    if (tab != NULL && key != NULL) {
        tab_item_t *item = (*tab)[get_hash(key)];
        tab_item_t *previous_item = NULL;
        while (item != NULL) {
            if (!strcmp(item->key, key)) {
                if (previous_item != NULL) {
                    previous_item->next_item = item->next_item;
                } else {
                    (*tab)[get_hash(key)] = (*tab)[get_hash(key)]->next_item;
                }
                free(item->data->identifier);
                free(item->data);
                free(item->key);
                free(item);
                return true;
            }
            previous_item = item;
            item = item->next_item;
        }
    }
    return false;
}

void delete_all_hashtable(table_t *tab) {
    // fully clear hash table and free allocated memory
    if (tab != NULL) {
        int i = 0;
        while (i < MAX_HT_SIZE) {
            tab_item_t *iter_item = (*tab)[i];
            tab_item_t *temp_iter_item;
            while (iter_item != NULL) {
                temp_iter_item = iter_item->next_item;
                free(iter_item->data->identifier);
                free(iter_item->data);
                free(iter_item->key);
                free(iter_item);
                iter_item = temp_iter_item;
            }
            (*tab)[i++] = NULL;
        }
    }
}

bool insert_parameter_item(tab_item_t *item, tab_item_data_type type) {
    if (item->data->item_id_type != FUNCTION) return false;
    item->data->item_parameters.type_parameters[item->data->item_parameters.count_parameters] = type;
    item->data->item_parameters.count_parameters++;
    if (item->data->item_parameters.count_parameters < MAX_PARAMETERS) return true;
    return false;
}

bool insert_return_item(tab_item_t *item, tab_item_data_type type) {
    if (item->data->item_id_type != FUNCTION) return false;
    item->data->item_returns.type_returns[item->data->item_returns.count_returns] = type;
    item->data->item_returns.count_returns++;
    if (item->data->item_returns.count_returns < MAX_RETURNS) return true;
    return false;
}
