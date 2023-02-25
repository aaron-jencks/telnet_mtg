#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

extern const int ERR_MEMORY;

void handle_memory_error(const char* filename, unsigned int linenum, const void* const ptr);

#endif