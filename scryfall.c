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
    char* temp = calloc(strlen(stritem->valuestring)+1, sizeof(char));
    temp = strcpy(temp, stritem->valuestring);
    return temp;
}

card_t init_card() {
    card_t result = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    return result;
}

card_t find_card(char* name) {
    char* sbuffer = malloc(sizeof(char) * (strlen(MTG_SDK_HOST) + strlen(name) + 20));
    sprintf(sbuffer, "%s/cards/named?exact=%s", MTG_SDK_HOST, name);
    api_response_t response = api_json_response(sbuffer);
    free(sbuffer);

    card_t cresponse = init_card();

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
            cresponse.faces = malloc(sizeof(card_t*)*(face_count+1));
            if (!cresponse.faces) {
                free(response.response);
                return cresponse;
            }
            cresponse.faces[face_count] = NULL;

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

    free(response.response);
    
    return cresponse;
}

void delete_card(card_t card) {
    if (card.id) free(card.id);
    if (card.name) free(card.name);
    if (card.manacost) free(card.manacost);
    if (card.type) free(card.type);
    if (card.text) free(card.text);
    if (card.power) free(card.power);
    if (card.toughness) free(card.toughness);
    if (card.loyalty) free(card.loyalty);
    if (card.faces) {
        for (card_t** f = (card_t**)card.faces; *f > 0; f++) {
            delete_card(**f);
            free(*f);
        }
        free(card.faces);
    }
}

char* display_card(card_t* card) {
    return card->name;
}

char* display_card_face(card_t* card, size_t face) {
    return ((card_t*)card->faces[face])->name;
}