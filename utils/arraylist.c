#include "arraylist.h"
#include "error_handler.h"
#include <stdlib.h>

arraylist_t create_arraylist(size_t capacity) {
    void** buff = (void**)malloc(sizeof(void*)*capacity);
    handle_memory_error("arraylist.c", 6, buff);
    arraylist_t arr = {buff, capacity, 0};
    return arr;
}

void destroy_arraylist(arraylist_t arr) {
    if (arr.arr) free(arr.arr);
}

size_t expand_arraylist(arraylist_t arr) {
    arr.capacity += 10;
    arr.arr = (void**)realloc(arr.arr, sizeof(void*) * arr.capacity);
    handle_memory_error("arraylist.c", 20, arr.arr);
    return arr.capacity;
}

size_t arraylist_append(arraylist_t arr, void* data) {
    if (arr.count == arr.capacity) expand_arraylist(arr);
    arr.arr[arr.count++] = data;
    return arr.count;
}

void* arraylist_index(arraylist_t arr, size_t index) {
    if (index >= arr.count) {
        return NULL;
    }
    return arr.arr[index];
}

void* arraylist_pop(arraylist_t arr) {
    if (arr.count == 0) return NULL;
    return arr.arr[--arr.count];
}

void* arraylist_remove(arraylist_t arr, size_t index) {
    if (index >= arr.count) return NULL;
    void* value = arr.arr[index];
    for (size_t i = index + 1; i < arr.count; i++) {
        arr.arr[i-1] = arr.arr[i];
    }
    arr.count--;
    return value;
}

size_t arraylist_insert(arraylist_t arr, size_t index, void* value) {
    if (arr.count + 1 >= arr.capacity) expand_arraylist(arr);
    if (index == arr.count) return arraylist_append(arr, value);
    if (index > arr.count) return arr.count;
    for(size_t i = arr.count-1; i != 0 && i >= index; i--) {
        arr.arr[i+1] = arr.arr[i];
    }
    if (index == 0) arr.arr[1] = arr.arr[0];
    arr.arr[index] = value;
    return ++arr.count;
}

void* arraylist_dequeue(arraylist_t arr) {
    return arraylist_remove(arr, 0);
}

size_t arraylist_len(arraylist_t arr) {
    if (arr.arr) return arr.count;
    return 0;
}

void* arraylist_last(arraylist_t arr) {
    if (arr.count) return arraylist_index(arr, arr.count-1);
    return NULL;
}