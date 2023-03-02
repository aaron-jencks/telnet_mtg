#ifndef UI_H
#define UI_H

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

#endif