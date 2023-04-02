#include <stdint.h>

#include "dict.h"
#include "error_handler.h"

int default_comparator(void* a, void* b) {
    return (int)((int64_t)a-(int64_t)b);
}

dict_t create_dict(size_t bcount, hashing_function hfunc) {
    dict_t returnValue;
    returnValue.bins = create_arraylist(bcount);
    for (size_t b = 0; b < bcount; b++)
        arraylist_append(returnValue.bins, create_parraylist(10));
    returnValue.count = 0;
    returnValue.hash_function = hfunc;
    returnValue.key_comparing_func = default_comparator;
    return returnValue;
}

void destroy_dict(dict_t dict) {
    while (arraylist_len(dict.bins)) {
        arraylist_t* bin = (arraylist_t*)arraylist_pop(dict.bins);
        while (arraylist_len(*bin)) free(arraylist_pop(*bin));
        destroy_arraylist(*bin);
        free(bin);
    }
    destroy_arraylist(dict.bins);
}

bool dict_contains(dict_t dict, void* key) {
    return dict_get(dict, key) != NULL;
}

void* dict_get(dict_t dict, void* key) {
    size_t bin = dict.hash_function(key);
    arraylist_t* b = (arraylist_t*)arraylist_index(dict.bins, bin);
    for (size_t kvp = 0; kvp < arraylist_len(*b); kvp++) {
        key_value_pair_t* kvpp = (key_value_pair_t*)arraylist_index(*b, kvp);
        if (!dict.key_comparing_func(kvpp->key, key)) return kvpp->value;
    }
    return NULL;
}

void dict_put(dict_t dict, void* key, void* value) {
    size_t bin = dict.hash_function(key);
    arraylist_t* b = (arraylist_t*)arraylist_index(dict.bins, bin);
    for (size_t kvp = 0; kvp < arraylist_len(*b); kvp++) {
        key_value_pair_t* kvpp = (key_value_pair_t*)arraylist_index(*b, kvp);
        if (!dict.key_comparing_func(kvpp->key, key)) {
            kvpp->value = value;
            return;
        }
    }
    key_value_pair_t* kvp = (key_value_pair_t*)malloc(sizeof(key_value_pair_t));
    handle_memory_error("dict.c", 54, kvp);
    kvp->key = key;
    kvp->value = value;
    arraylist_append(*b, kvp);
}

void* dict_remove(dict_t dict, void* key) {
    size_t bin = dict.hash_function(key);
    arraylist_t* b = (arraylist_t*)arraylist_index(dict.bins, bin);
    for (size_t kvp = 0; kvp < arraylist_len(*b); kvp++) {
        key_value_pair_t* kvpp = (key_value_pair_t*)arraylist_index(*b, kvp);
        if (!dict.key_comparing_func(kvpp->key, key)) {
            arraylist_remove(*b, kvp);
            void* v = kvpp->value;
            free(kvpp);
            return v;
        }
    }
    return NULL;
}
