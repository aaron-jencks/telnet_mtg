#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

size_t menu(char* title, char* description, char** entries, size_t entry_count) {
    printf("%s\n%s\n", title, description);
    for(size_t i = 0; i < entry_count; i++) {
        printf("%ld:\t%s", i, entries[i]);
    }
    printf("Please enter a selection (0-%ld): ", entry_count-1);
    size_t response;
    while (1) {
        scanf("%ld", &response);
        if (response >= entry_count) printf("That response is invalid, please try again...\n");
        else break;
    }
    return response;
}