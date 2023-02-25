#include "scryfall.h"
#include "https_client/https.h"
#include "cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

char* parse_json_for_string(cJSON* item, char* name) {
    cJSON* stritem = cJSON_GetObjectItemCaseSensitive(item, name);
    if (stritem == NULL || !(cJSON_IsString(stritem) && stritem->valuestring != NULL)) {
        return NULL;
    }
    return stritem->valuestring;
}

card_t find_card(char* name) {
    char* sbuffer = malloc(sizeof(char) * (strlen(MTG_SDK_HOST) + strlen(name) + 20));
    sprintf(sbuffer, "%s/cards/named?exact=%s", MTG_SDK_HOST, name);
    api_response_t response = api_json_response(sbuffer);

    card_t cresponse;
    cresponse.name = "NOT FOUND";

    if (response.http_code == 200) {
        cJSON* json = cJSON_Parse(response.response);

        if (!json) {
            return cresponse;
        }

        char* strings[] = {
            "id",
            "name",
            "mana_cost",
            "type_line",
            "oracle_text",
            "power",
            "toughness",
            "loyalty"
        };

        char** props[] = {
            &cresponse.id,
            &cresponse.name,
            &cresponse.manacost,
            &cresponse.type,
            &cresponse.text,
            &cresponse.power,
            &cresponse.toughness,
            &cresponse.loyalty
        };

        char* temp;

        for (int i = 0; i < 8; i++) {
            temp = parse_json_for_string(json, strings[i]);
            *(props[i]) = temp;
        }

        cJSON* faces = cJSON_GetObjectItemCaseSensitive(json, "card_faces");
        if (faces) {
            cJSON* face;
            size_t face_count = 0;
            cJSON_ArrayForEach(face, faces) face_count++;
            cresponse.faces = malloc(sizeof(card_t*)*face_count);
            if (!cresponse.faces) return cresponse;

            size_t findex = 0;
            cJSON_ArrayForEach(face, faces) {
                card_t* fst = malloc(sizeof(card_t));
                char** fprops[] = {
                    &fst->name,
                    &fst->manacost,
                    &fst->type,
                    &fst->text,
                    &fst->power,
                    &fst->toughness,
                    &fst->loyalty
                };

                for (int i = 1; i < 8; i++) {
                    temp = parse_json_for_string(json, strings[i]);
                    *(fprops[i-1]) = temp;
                }

                cresponse.faces[findex++] = fst;
            }
        }

        cJSON_Delete(json);
    }
    
    return cresponse;
}

char* display_card(card_t* card) {
    return card->name;
}

char* display_card_face(card_t* card, size_t face) {
    return ((card_t*)card->faces[face])->name;
}