#ifndef MTGSDK_H
#define MTGSDK_H

#include <stddef.h>

extern const char* MTG_SDK_HOST;
extern const int RESPONSE_BUFFER_SIZE;

typedef struct {
    int http_code;
    char* response;
} api_response_t;

typedef struct {

    char* id;
    char* name;
    char* manacost;
    int cmc;
    char* type;
    void** faces;
    char* text;
    char* power;
    char* toughness;
    char* loyalty;

} card_t;

typedef struct {
    card_t* cards;
    size_t len;
} card_search_result_t;

/**
 * Fetches the response from the api and returns it's content if any
 * @param url The url to send to the api
 * @returns the raw json string returned by the request, or NULL if an error occured
*/
api_response_t api_json_response(char* url);

/**
 * Fetches a card from the api if it can find one
 * @param name The name of the card to search for
 * @returns a struct representing the desired card, or NULL if not found
*/
card_t scryfall_find(char* name);

/**
 * @brief Fetches a list of cards from the api that are similar to the supplied keyword
 * 
 * @param keyword They keyword to look for
 * @return card_t* A zero-terminated list of cards
 */
card_search_result_t scryfall_search(char* keyword);

/**
 * Creates a new card with all of the pointers set to NULL
*/
card_t init_card();

/**
 * @brief Deletes a search result and all of it's elements
 * 
 * @param sr the search result to de-alloc
 */
void delete_search_result(card_search_result_t sr);

/**
 * Frees the allocated memory of the card and any child cards
 * @param card The card to deallocate
*/
void delete_card(card_t card);

/**
 * Converts the given card struct to a string for printing
 * The caller must free the result
*/
char* display_card(card_t* card);

/**
 * A similar form of display_card, 
 * but displays a specific face of the card
 * The caller must free the result
*/
char* display_card_face(card_t* card, size_t face);

#endif