#include <stdint.h>
#include <error.h>
#include <stdlib.h>

#include "dict.h"
#include "error_handler.h"

int default_comparator(void* a, void* b) {
    return (int)((int64_t)a-(int64_t)b);
}

dict_t create_dict(size_t bcount, hashing_function hfunc) {
    dict_t returnValue;
    returnValue.bins = create_parraylist(bcount);
    for (size_t b = 0; b < bcount; b++)
        arraylist_append(returnValue.bins, create_parraylist(10));
    returnValue.count = 0;
    returnValue.rehash_threshold = 0.75;
    returnValue.hash_function = hfunc;
    returnValue.key_comparing_func = default_comparator;
    return returnValue;
}

void destroy_dict(dict_t dict) {
    while (arraylist_len(*dict.bins)) {
        arraylist_t* bin = (arraylist_t*)arraylist_pop(dict.bins);
        while (arraylist_len(*bin)) free(arraylist_pop(bin));
        destroy_arraylist(*bin);
        free(bin);
    }
    destroy_arraylist(*dict.bins);
    free(dict.bins);
}

bool dict_contains(dict_t dict, void* key) {
    return dict_get(dict, key) != NULL;
}

arraylist_t* get_bin_for_key(dict_t dict, void* key) {
    size_t bin = dict.hash_function(key) % dict.bins->count;
    return (arraylist_t*)arraylist_index(*dict.bins, bin);
}

void* dict_get(dict_t dict, void* key) {
    arraylist_t* b = get_bin_for_key(dict, key);
    for (size_t kvp = 0; kvp < arraylist_len(*b); kvp++) {
        key_value_pair_t* kvpp = (key_value_pair_t*)arraylist_index(*b, kvp);
        if (!dict.key_comparing_func(kvpp->key, key)) return kvpp->value;
    }
    return NULL;
}

void dict_put(dict_t* dict, void* key, void* value) {
    arraylist_t* b = get_bin_for_key(*dict, key);
    for (size_t kvp = 0; kvp < arraylist_len(*b); kvp++) {
        key_value_pair_t* kvpp = (key_value_pair_t*)arraylist_index(*b, kvp);
        if (!dict->key_comparing_func(kvpp->key, key)) {
            kvpp->value = value;
            return;
        }
    }
    key_value_pair_t* kvp = (key_value_pair_t*)malloc(sizeof(key_value_pair_t));
    handle_memory_error("dict.c", 61, kvp);
    kvp->key = key;
    kvp->value = value;
    arraylist_append(b, kvp);
    dict->count++;

    if (dict_load_factor(*dict) > dict->rehash_threshold) 
        dict_resize(*dict, dict->bins->count + 10);
}

void* dict_remove(dict_t* dict, void* key) {
    arraylist_t* b = get_bin_for_key(*dict, key);
    for (size_t kvp = 0; kvp < arraylist_len(*b); kvp++) {
        key_value_pair_t* kvpp = (key_value_pair_t*)arraylist_index(*b, kvp);
        if (!dict->key_comparing_func(kvpp->key, key)) {
            arraylist_remove(b, kvp);
            void* v = kvpp->value;
            dict->count--;
            free(kvpp);
            return v;
        }
    }
    return NULL;
}

arraylist_t dict_to_list(dict_t dict, bool empty) {
    arraylist_t result = create_arraylist(dict.count);
    for (size_t bi = 0; bi < dict.bins->count; bi++) {
        arraylist_t* bin = (arraylist_t*)arraylist_index(*dict.bins, bi);
        if (empty) {
            while (arraylist_len(*bin)) 
                arraylist_append(&result, arraylist_pop(bin));
        } else {
            for (size_t bv = 0; bv < bin->count; bv++)
                arraylist_append(&result, arraylist_index(*bin, bv));
        }
    }
    return result;
}

void dict_resize(dict_t dict, size_t new_size) {
    if (new_size <= dict.bins->count) {
        error_at_line(ERR_DICT_SIZE, 0, "dict.c", 103, "New dict size must be larger than previous size");
    }

    // Empty the dict
    arraylist_t contents = dict_to_list(dict, true);

    // Create new bins
    size_t diff = new_size - dict.bins->count;
    for (size_t b = 0; b++ < diff;) arraylist_append(dict.bins, create_parraylist(10));

    // Rehash
    while (arraylist_len(contents)) {
        key_value_pair_t* kvp = (key_value_pair_t*)arraylist_pop(&contents);
        arraylist_t* bin = get_bin_for_key(dict, kvp->key);
        arraylist_append(bin, (void*)kvp);
    }
}

float dict_load_factor(dict_t dict) {
    return (float)dict.count / (float)dict.bins->count;
}
