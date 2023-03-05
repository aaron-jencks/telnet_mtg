#include "scryfall.h"
#include "https_client/https.h"
#include "../utils/cJSON.h"
#include "../utils/urlencode.h"
#include "../utils/error_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>

const char* MTG_SDK_HOST = "https://api.scryfall.com";
const int RESPONSE_BUFFER_SIZE = 8192;

api_response_t api_json_response(char* url) {
    // reaches out to the scryfall for the info
    int current_buff_size = RESPONSE_BUFFER_SIZE, ret;

    char* response = malloc(current_buff_size * sizeof(char));
    handle_memory_error("scryfall.c", 16, response);

    HTTP_INFO hi;

    http_init(&hi, FALSE);
    while (TRUE) {
        ret = http_get(&hi, url, response, current_buff_size);
        if (hi.response.content_length > current_buff_size) {
            // We have an incomplete response
            current_buff_size = hi.response.content_length;
            response = realloc(response, sizeof(char) * current_buff_size);
            handle_memory_error("scryfall.c", 28, response);
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
    handle_memory_error("scryfall.c", 48, temp);
    temp = strcpy(temp, stritem->valuestring);
    return temp;
}

card_t init_card() {
    card_t result = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    return result;
}

card_t find_card(char* name) {
    char* encoded_name = calloc((3 * strlen(name) + 1), sizeof(char));
    handle_memory_error("scryfall.c", 60, encoded_name);
    url_encode(html5, name, encoded_name);
    char* sbuffer = malloc(sizeof(char) * (strlen(MTG_SDK_HOST) + strlen(encoded_name) + 20));
    handle_memory_error("scryfall.c", 63, sbuffer);
    sprintf(sbuffer, "%s/cards/named?exact=%s", MTG_SDK_HOST, encoded_name);
    api_response_t response = api_json_response(sbuffer);
    free(encoded_name);
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
            handle_memory_error("scryfall.c", 113, cresponse.faces);
            if (!cresponse.faces) {
                free(response.response);
                return cresponse;
            }
            cresponse.faces[face_count] = NULL;

            size_t findex = 0;
            cJSON_ArrayForEach(face, faces) {
                card_t* fst = malloc(sizeof(card_t));
                handle_memory_error("scryfall.c", 123, fst);
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
    } else {
        error_at_line(3, ERR_SCRYFALL, "scryfall.c", 66, "Scryfall server responded with: %d\n%s", response.http_code, response.response);
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
    size_t tlen = 4;
    char *name, *cost, *type, *text=0, *power=0, *toughness=0, *loyalty=0;

    if (card->name) {
        tlen += strlen(card->name);
        name = card->name;
    } else {
        tlen += 4;
        name = "NONE";
    }

    if (card->manacost) {
        tlen += strlen(card->manacost);
        cost = card->manacost;
    } else {
        tlen += 1;
        cost = "0";
    }

    if (card->type) {
        tlen += strlen(card->type);
        type = card->type;
    } else {
        tlen += 4;
        type = "NONE";
    }

    if (card->text) {
        tlen += strlen(card->text);
        text = card->text;
    }

    if (card->power) {
        tlen += strlen(card->power);
        power = card->power;
    }

    if (card->toughness) {
        tlen += strlen(card->toughness);
        toughness = card->toughness;
    }

    if (card->loyalty) {
        tlen += strlen(card->loyalty);
        loyalty = card->loyalty;
    }

    char *template = "%s\t%s\n%s\n%s", *pttemplate = "(%s/%s)", *ltemplate = "(%s)", *finaltemplate;
    char allocd = 1;
    size_t tsize = strlen(template) + 1;
    if (power && toughness) {
        tsize += strlen(pttemplate) + 1;
        finaltemplate = malloc(sizeof(char) * tsize);
        handle_memory_error("scryfall.c", 223, finaltemplate);
        finaltemplate[tsize-1] = 0;
        sprintf(finaltemplate, "%s\n%s", template, pttemplate);
        tlen += 4;
    } else if (loyalty) {
        tsize += strlen(ltemplate) + 1;
        finaltemplate = malloc(sizeof(char) * tsize);
        handle_memory_error("scryfall.c", 230, finaltemplate);
        finaltemplate[tsize-1] = 0;
        sprintf(finaltemplate, "%s\n%s", template, ltemplate);
        tlen += 3;
    } else {
        finaltemplate = template;
        allocd = 0;
    }

    char* result = malloc(sizeof(char) * tlen);
    handle_memory_error("scryfall.c", 240, result);
    result[tlen-1]=0;

    if (power && toughness) {
        sprintf(result, finaltemplate, name, cost, type, text ? text : "", power, toughness);
    } else if (loyalty) {
        sprintf(result, finaltemplate, name, cost, type, text ? text : "", loyalty);
    } else {
        sprintf(result, finaltemplate, name, cost, type, text ? text : "");
    }

    if (allocd) free(finaltemplate);

    return result;
}

char* display_card_face(card_t* card, size_t face) {
    if (!card->faces || !face) {
        return display_card(card);
    }
    return display_card((card_t*)card->faces[face]);
}