#include "scryfall.h"
#include "https_client/https.h"
#include <stdlib.h>
#include <stdio.h>

const char* MTG_SDK_HOST = "https://api.scryfall.com";
const int RESPONSE_BUFFER_SIZE = 8192;

api_response_t api_json_response(char* url) {
    // reaches out to the gathering.io for the info
    int current_buff_size = RESPONSE_BUFFER_SIZE, ret;
    char* response = malloc(current_buff_size * sizeof(char));
    HTTP_INFO hi;

    http_init(&hi, FALSE);
    while (TRUE) {
        ret = http_get(&hi, url, response, current_buff_size);
        if (hi.response.content_length > current_buff_size) {
            // We have an incomplete response
            current_buff_size = hi.response.content_length;
            response = realloc(response, sizeof(char) * current_buff_size);
            continue;
        }
        break;
    }
    http_close(&hi);

    api_response_t result;
    result.http_code = ret;
    result.response = response;

    return result;
}

card_t find_card(char* name) {

}