#ifndef UI_H
#define UI_H

#include "utils/arraylist.h"
#include <stddef.h>

/**
 * Represents the response of interacting with a window
*/
typedef struct {
    void* window; // The window to push onto the stack, if NULL the no window is pushed. Must be of type window_t
    char keep; // Determines whether to keep the parent window, or pop it off.
} interaction_response_t;

/**
 * Represents a window that the user can interact with
*/
typedef struct {
    /**
     * A function for interacting with the window
    */
    interaction_response_t (*interact)();
} window_t;

/**
 * Contains the window stack that a user has
*/
typedef struct {
    /**
     * The current stack of windows
     * The window on top is the one currently being displayed
    */
    arraylist_t stack;
} window_controller_t;

/**
 * Displays a menu for the user to interact with
 * This loops until the user selects a valid entry
 * @param title The title bar displayed at the top of the menu
 * @param description The description that describes the title
 * @param entries The list of selectable options, these will be enumerated automatically
 * @param entry_count The number of entries in the entries parameter
 * @returns Returns the index of the entry selected
*/
size_t menu(char* title, char* description, char** entries, size_t entry_count);

/**
 * Creates a new window with the given interaction function
*/
window_t* create_window(interaction_response_t (*handler)());

/**
 * Creates a new window controller with it's own stack
 * @returns Returns the new window controller
*/
window_controller_t create_window_controller();

/**
 * Deallocates the arraylist used for the stack of the window controller.
*/
void destroy_window_controller(window_controller_t wc);

/**
 * Pushes a new window onto the stack.
 * It is now the currently displayed window.
*/
void wc_push_window(window_controller_t wc, window_t* window);

/**
 * Pops the window off the top of the stack.
 * The window below it is the one now being displayed.
*/
window_t* wc_pop_window(window_controller_t wc);

/**
 * Runs the ui loop
 * There needs to be at least a single window on the stack before starting
 * Loops by displaying the top window and handling the return value
 * If the return value of the interaction is NULL then no window is put in it's place
 * otherwise it pushes that window onto the stack and repeats
*/
void wc_loop(window_controller_t wc);

#endif