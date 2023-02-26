#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

extern const int ERR_MEMORY;
extern const int ERR_DB;
extern const int ERR_SCRYFALL;

void handle_memory_error(const char* filename, unsigned int linenum, const void* const ptr);

#endif