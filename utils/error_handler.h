#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

extern const int ERR_MEMORY;
extern const int ERR_DB;
extern const int ERR_SCRYFALL;
extern const int ERR_DICT_SIZE;
extern const int ERR_MUTEX_INIT;
extern const int ERR_PTHREAD;

void handle_memory_error(const char* filename, unsigned int linenum, const void* const ptr);

#endif