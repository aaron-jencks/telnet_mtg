#include "scryfall.h"
#include "https_client/https.h"
#include "../utils/cJSON.h"
#include "../utils/urlencode.h"
#include "../utils/error_handler.h"
#include "../utils/arraylist.h"
#include "../utils/strings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <math.h>

const char* MTG_SDK_HOST = "https://api.scryfall.com";
const int RESPONSE_BUFFER_SIZE = 8192;

api_response_t api_json_response(char* url) {
    // reaches out to the scryfall for the info
    int current_buff_size = RESPONSE_BUFFER_SIZE, ret;

    char* response = malloc((current_buff_size + 1) * sizeof(char));
    handle_memory_error("scryfall.c", 21, response);

    HTTP_INFO hi;

    http_init(&hi, FALSE);
    while (TRUE) {
        response[current_buff_size] = 0;
        ret = http_get(&hi, url, response, current_buff_size);
        if (hi.response.content_length > current_buff_size) {
            // We have an incomplete response
            current_buff_size = hi.response.content_length + 1;
            response = realloc(response, sizeof(char) * (current_buff_size + 1));
            handle_memory_error("scryfall.c", 33, response);
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
    char* temp = strreplace(stritem->valuestring, "\n", "\r\n");
    return temp;
}

double parse_json_for_number(cJSON* item, char* name) {
    cJSON* stritem = cJSON_GetObjectItemCaseSensitive(item, name);
    return cJSON_GetNumberValue(stritem);
}

card_t init_card() {
    card_t result = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    return result;
}

card_t parse_card_json(cJSON* json) {
    card_t cresponse = init_card();

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
        handle_memory_error("scryfall.c", 110, cresponse.faces);
        if (!cresponse.faces) {
            return cresponse;
        }
        cresponse.faces[face_count] = NULL;

        size_t findex = 0;
        cJSON_ArrayForEach(face, faces) {
            card_t* fst = malloc(sizeof(card_t));
            handle_memory_error("scryfall.c", 117, fst);
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

    return cresponse;
}

card_t scryfall_find(char* name) {
    char* encoded_name = calloc((3 * strlen(name) + 1), sizeof(char));
    handle_memory_error("scryfall.c", 142, encoded_name);
    url_encode(html5, name, encoded_name);
    char* sbuffer = malloc(sizeof(char) * (strlen(MTG_SDK_HOST) + strlen(encoded_name) + 20));
    handle_memory_error("scryfall.c", 145, sbuffer);
    sprintf(sbuffer, "%s/cards/named?exact=%s", MTG_SDK_HOST, encoded_name);
    api_response_t response = api_json_response(sbuffer);
    free(encoded_name);
    free(sbuffer);

    card_t cresponse = init_card();

    if (response.http_code == 200) {
        cJSON* json = cJSON_Parse(response.response);

        cresponse = parse_card_json(json);

        cJSON_Delete(json);
    } else {
        error_at_line(ERR_SCRYFALL, ERR_SCRYFALL, "scryfall.c", 148, "Scryfall server responded with: %d\n%s", response.http_code, response.response);
    }

    free(response.response);
    
    return cresponse;
}

card_search_result_t scryfall_search(char* keyword) {
    char* encoded_keyword = calloc((3 * strlen(keyword) + 1), sizeof(char));
    handle_memory_error("scryfall.c", 170, encoded_keyword);
    url_encode(html5, keyword, encoded_keyword);
    char* sbuffer = malloc(sizeof(char) * (strlen(MTG_SDK_HOST) + strlen(encoded_keyword) + 20));
    handle_memory_error("scryfall.c", 173, sbuffer);
    sprintf(sbuffer, "%s/cards/search?q=%s", MTG_SDK_HOST, encoded_keyword);
    api_response_t response = api_json_response(sbuffer);
    free(encoded_keyword);
    free(sbuffer);

    card_t* result_buff = (card_t*)malloc(sizeof(card_t)*10);
    handle_memory_error("scryfall.c", 180, result_buff);
    size_t result_buff_index = 0;

    if (response.http_code == 200) {
        char* json_error;
        cJSON* json = cJSON_ParseWithOpts(response.response, &json_error, 0);

        if (!json) {
            free(result_buff);
            return (card_search_result_t){NULL, 0};
        }

        double card_count_d = parse_json_for_number(json, "total_cards");
        cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");

        if (!data) {
            free(result_buff);
            return (card_search_result_t){NULL, 0};
        }

        for (size_t card_i = 0; card_i < ((card_count_d > 10) ? 10 : card_count_d); card_i++) {
            card_t cresponse = init_card();

            cJSON* arr_item = cJSON_GetArrayItem(data, card_i);
            cresponse = parse_card_json(arr_item);

            result_buff[result_buff_index++] = cresponse;
        }

        cJSON_Delete(json);
    } else {
        error_at_line(ERR_SCRYFALL, ERR_SCRYFALL, "scryfall.c", 176, "Scryfall server responded with: %d\n%s", response.http_code, response.response);
    }

    free(response.response);
    
    return (card_search_result_t){
        result_buff,
        result_buff_index
    };
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
    size_t tlen = 6;
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

    char *template = "%s\t%s\r\n%s\r\n%s", *pttemplate = "(%s/%s)", *ltemplate = "(%s)", *finaltemplate;
    char allocd = 1;
    size_t tsize = strlen(template) + 1;
    if (power && toughness) {
        tsize += strlen(pttemplate) + 2;
        finaltemplate = malloc(sizeof(char) * tsize);
        handle_memory_error("scryfall.c", 288, finaltemplate);
        finaltemplate[tsize-1] = 0;
        sprintf(finaltemplate, "%s\r\n%s", template, pttemplate);
        tlen += 5;
    } else if (loyalty) {
        tsize += strlen(ltemplate) + 2;
        finaltemplate = malloc(sizeof(char) * tsize);
        handle_memory_error("scryfall.c", 295, finaltemplate);
        finaltemplate[tsize-1] = 0;
        sprintf(finaltemplate, "%s\r\n%s", template, ltemplate);
        tlen += 4;
    } else {
        finaltemplate = template;
        allocd = 0;
    }

    char* result = malloc(sizeof(char) * tlen);
    handle_memory_error("scryfall.c", 305, result);
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