#ifndef MTGSDK_H
#define MTGSDK_H

const char* MTG_SDK_HOST;
const int RESPONSE_BUFFER_SIZE;

typedef struct {
    int http_code;
    char* response;
} api_response_t;

typedef struct {

    char* name;
    char* manacost;
    int cmc;
    char* type;
    char** names;
    char** rulings;
    char* text;
    char* layout;
    char* multiverseid;
    char* power;
    char* toughness;
    char* loyalty;

} card_t;

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
card_t find_card(char* name);

#endif