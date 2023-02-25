#include <error.h>
#include "error_handler.h"

const int ERR_MEMORY = 0;

void handle_memory_error(const char* filename, unsigned int linenum, const void* const ptr) {
    if (!ptr) error_at_line(1, ERR_MEMORY, filename, linenum, "Out of Memory!");
}