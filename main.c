#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scryfall.h"


int main(int argc, char *argv[]) {
    char* sbuffer = malloc(sizeof(char) * strlen(MTG_SDK_HOST) + 64);
    sprintf(sbuffer, "%s/%s", MTG_SDK_HOST, "/cards/named?exact=solitude");
    api_response_t response = api_json_response(sbuffer);
    printf("return code: %d \n", response.http_code);
    printf("return body: %s \n", response.response);
    return 0;
}
