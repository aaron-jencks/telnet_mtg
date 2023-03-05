#include "ui.h"
#include "utils/error_handler.h"
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

window_t* create_window(interaction_response_t (*handler)()) {
    window_t* result = malloc(sizeof(window_t));
    handle_memory_error("ui.c", 22, result);
    result->interact = handler;
    return result;
}

window_controller_t create_window_controller() {
    window_controller_t result = {
        create_arraylist(10)
    };
    return result;
}

void destroy_window_controller(window_controller_t wc) {
    destroy_arraylist(wc.stack);
}

void wc_push_window(window_controller_t wc, window_t* window) {
    arraylist_append(wc.stack, window);
}

window_t* wc_pop_window(window_controller_t wc) {
    return (window_t*)arraylist_pop(wc.stack);
}

void wc_loop(window_controller_t wc) {
    while (wc.stack.count) {
        window_t* top = arraylist_last(wc.stack);
        if (!top) continue;
        interaction_response_t resp = top->interact();
        if (!resp.keep) arraylist_pop(wc.stack);
        if (resp.window) arraylist_append(wc.stack, resp.window);
    }
}