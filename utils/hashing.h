#ifndef HASHING_H
#define HASHING_H

#include <stddef.h>

/**
 * @brief Hashes the given string using the prime number 31
 * Source: https://stackoverflow.com/questions/2624192/good-hash-function-for-strings
 * 
 * @return size_t 
 */
size_t hash_string_prime(void* s);

#endif