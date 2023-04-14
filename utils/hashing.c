#include <string.h>

#include "hashing.h"

size_t hash_string_prime(void* s) {
    char* cs = (char*)s;
    size_t returnValue = 0xdeadbeef;
    for(size_t i = 0; i < strlen(cs); i++) {
        returnValue = returnValue * 31 + cs[i];
    }
    return returnValue;
}
