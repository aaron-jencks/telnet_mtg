#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stddef.h>

typedef struct {
    void** arr;
    size_t capacity;
    size_t count;
} arraylist_t;

/**
 * Creates an arraylist with the given initial capacity
 * @param capacity the initial capacity to use.
 * @returns Returns a new arraylist
*/
arraylist_t create_arraylist(size_t capacity);

/**
 * Deallocates any memory of the given arraylist
 * @param arr the arraylist to deallocate
*/
void destroy_arraylist(arraylist_t arr);

/**
 * Appends an element to the end of the arraylist
 * @param arr the arraylist to be appended to
 * @param data the value to append to the end of the list
 * @returns Returns the new size of the arraylist.
*/
size_t arraylist_append(arraylist_t arr, void* data);

/**
 * Retrieves the value at the given index of the arraylist
 * Returns NULL if the index is out of bounds
 * @param arr The arraylist to index
 * @param index the index to retrieve the value of
 * @returns Returns the value of the index or NULL if out of bounds
*/
void* arraylist_index(arraylist_t arr, size_t index);

/**
 * Pops the last element of the arraylist off and returns it
 * @param arr The array to pop the elemtn off of
 * @returns Returns the value that was popped off, or NULL if the previous count was 0.
*/
void* arraylist_pop(arraylist_t arr);

/**
 * Removes the value at the given index
 * @param arr the arraylist to remove the value from
 * @param index the index to remove the value from
 * @returns Returns the removed value, or NULL if the index was out of bounds
*/
void* arraylist_remove(arraylist_t arr, size_t index);

/**
 * Inserts a new value in the arraylist at the given index
 * @param arr The arraylist to insert the value into
 * @param index the index to place the value at
 * @param value The value to place at the given index
 * @returns Returns the new size of the array, 
 * in the case that the index is out of bounds, then the size will not change.
*/
size_t arraylist_insert(arraylist_t arr, size_t index, void* value);

/**
 * Removes the first element of the array
 * @param arr the array to dequeue the element from
 * @returns Returns the dequeued element, or NULL if the size of the arraylist was 0
*/
void* arraylist_dequeue(arraylist_t arr);

/**
 * Returns the number of elements in the arraylist,
 * or zero if the buffer isn't allocated yet
*/
size_t arraylist_len(arraylist_t arr);

/**
 * Returns the last element of the array, or NULL
 * if there aren't any elements in the arraylist
*/
void* arraylist_last(arraylist_t arr);

#endif