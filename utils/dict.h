#ifndef DICT_H
#define DICT_H

#include <stddef.h>
#include <stdbool.h>

#include "./arraylist.h"

typedef size_t (*hashing_function)(void*);

/**
 * @brief Represents a comparator, compares the two input values and
 * @returns 0 if equal, <0 if arg1 < arg2, and >0 if arg1 > arg2
 * 
 */
typedef int (*comparator)(void*, void*);

typedef struct {
    void* key;
    void* value;
} key_value_pair_t;

typedef struct {
    arraylist_t bins;
    hashing_function hash_function;
    comparator key_comparing_func;
    size_t count;
    float rehash_threshold;
} dict_t;

/**
 * @brief The default pointer comparator evaluates the literal pointer values
 * 
 * @param a 
 * @param b 
 * @return int (int)(a-b)
 */
int default_comparator(void* a, void* b);

/**
 * Creates a new dictionary with the given number of bins and the given hashing function
 * @param bcount The number of bins to use by default
 * @param hfunc The hashing function to use
 * @returns Returns a new initialized dictionary struct
*/
dict_t create_dict(size_t bcount, hashing_function hfunc);

/**
 * @brief De-allocates the contents of a dictionary
 * 
 * @param dict The dictionary to de-allocate
 */
void destroy_dict(dict_t dict);

/**
 * @brief Checks if the given key contains a non-null value in the dictionary
 * 
 * @param dict The dictionary to look in
 * @param key The key to check for
 * @return true if the key exists in the dictionary
 * @return false if the key does not exist in the dictionary
 */
bool dict_contains(dict_t dict, void* key);

/**
 * @brief retrieves the value for the given key
 * 
 * @param dict The dictionary to look in
 * @param key the key to retrieve the value for
 * @return void* Returns the value corresponding to the key or NULL if the key does not exist
 */
void* dict_get(dict_t dict, void* key);

/**
 * @brief Inserts or replaces the given value into the dictionary with the given key
 * 
 * @param dict The dictionary to insert into
 * @param key The key for the value to insert
 * @param value The value to assign to the given key
 */
void dict_put(dict_t dict, void* key, void* value);

/**
 * @brief Deletes the given key from the dictionary if it exists
 * 
 * @param dict The dictionary to remove from
 * @param key The key to delete from the dictionary
 * @return void* Returns the deleted value, or NULL if it didn't exist
 */
void* dict_remove(dict_t dict, void* key);

/**
 * @brief Rehashes the current dict and creates additional bins
 * 
 * @param dict the dict to rehash
 * @param new_size the new number of bins to contain
 */
void dict_resize(dict_t dict, size_t new_size);

/**
 * @brief Returns the load factor of the dictionary
 * 
 * @param dict the dictionary to return the laod factor for
 * @return float Returns the load factor
 */
float dict_load_factor(dict_t dict);

#endif