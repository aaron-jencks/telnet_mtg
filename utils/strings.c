#include "strings.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "error_handler.h"

size_t strcount(char* s, char* delim) {
    size_t returnValue;
    for(char* si = s; *si; si++) {
        for (char* di = delim; *di; di++) {
            if(*si == *di) {
                returnValue++;
                break;
            }
        }
    }
    return returnValue;
}

char* strreplace(char* s, char* delim, char* sub) {
    size_t count = strcount(s, delim);
    size_t sucount = strlen(sub);
    size_t scount = strlen(s);
    size_t ncount = scount + count * (sucount - 1) + 1;

    char* returnValue = malloc(sizeof(char) * ncount);
    handle_memory_error("strings.c", 28, returnValue);
    returnValue[ncount-1] = 0;

    size_t soffset = 0;
    for(size_t ni = 0; ni < ncount;) {
        bool found = false;
        for (char* di = delim; *di; di++) {
            if(s[soffset] == *di) {
                found = true;
                for(char* sui = sub; *sui; sui++) {
                    returnValue[ni++] = *sui;
                }
                soffset++;
                break;
            }
        }
        if(found) continue;
        returnValue[ni++] = s[soffset++];
    }


    return returnValue;
}
