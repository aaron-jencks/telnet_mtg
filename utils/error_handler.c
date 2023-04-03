#include <error.h>
#include "error_handler.h"

const int ERR_MEMORY = 0;
const int ERR_DB = 1;
const int ERR_SCRYFALL = 2;
const int ERR_DICT_SIZE = 3;
const int ERR_MUTEX_INIT = 4;
const int ERR_PTHREAD = 5;

void handle_memory_error(const char* filename, unsigned int linenum, const void* const ptr) {
    if (!ptr) error_at_line(1, ERR_MEMORY, filename, linenum, "Out of Memory!");
}