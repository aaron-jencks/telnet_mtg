#ifndef DICT_H
#define DICT_H

#include <stddef.h>
#include <stdbool.h>

#include "./arraylist.h"

typedef size_t (*hashing_function)(void*);

typedef struct {
    arraylist_t bins;
    hashing_function hash_function;
    size_t count;
} dict_t;

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
 * @param key The key to check for
 * @return true if the key exists in the dictionary
 * @return false if the key does not exist in the dictionary
 */
bool dict_contains(void* key);

/**
 * @brief retrieves the value for the given key
 * 
 * @param key the key to retrieve the value for
 * @return void* Returns the value corresponding to the key or NULL if the key does not exist
 */
void* dict_get(void* key);

/**
 * @brief Inserts or replaces the given value into the dictionary with the given key
 * 
 * @param key The key for the value to insert
 * @param value The value to assign to the given key
 */
void dict_put(void* key, void* value);

/**
 * @brief Deletes the given key from the dictionary if it exists
 * 
 * @param key The key to delete from the dictionary
 * @return void* Returns the deleted value, or NULL if it didn't exist
 */
void* dict_remove(void* key);

#endif