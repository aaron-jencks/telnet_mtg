#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

/**
 * @brief Counts how many characters are in s that are in delim
 * 
 * @param s The string to count occurrences of
 * @param delim Bytes to include in the count
 * @return size_t Returns the number of occurrences of bytes in delim, in s
 */
size_t strcount(char* s, char* delim);

/**
 * @brief Replaces any character in delim, found in s, with sub
 * 
 * @param s The string to replace occurrences in
 * @param delim Bytes to replace
 * @param sub String to replace bytes with
 * @return char* A new string with instances replaced, must be freed by the caller
 */
char* strreplace(char* s, char* delim, char* sub);

#endif