#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scryfall.h"
#include "urlencode.h"


int main(int argc, char *argv[]) {
    url_encoder_rfc_tables_init();
    card_t c = find_card("Cathar's Crusade");
    char* dstring = display_card(&c);
    printf("%s\n", dstring);
    free(dstring);
    delete_card(c);
    return 0;
}
