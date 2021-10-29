#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"

static unsigned get_hash(const char *str)
{
    //TODO hash generator
}

void init_hashtab(table_t *tab)
{
    if (tab != NULL)
    {
        int i = 0;
        while (i < MAX_HT_SIZE)
        {
            (*tab)[i++] = NULL;
        }
    }
}

void delete_all_hashtab(table_t *tab)
{
    if (tab != NULL) {
        int i = 0;
        while (i < MAX_HT_SIZE) {
            table_item_t *iter_item = (*tab)[i];
            while (iter_item != NULL) {
                table_item_t *temp_iter_item = iter_item->next;
                if ((iter_item->value.parametrs != NULL))
                {
                    string_free(iter_item->value.parametrs);
                    free(iter_item->value.parametrs);
                }
                free(iter_item->key);
                free(iter_item);
                iter_item = temp_iter_item;
            }
            (*tab)[i++] = NULL;
        }
    }
}

//TODO