#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scryfall.h"
#include "urlencode.h"
#include "sqlite_wrapper.h"


int main(int argc, char *argv[]) {
    create_tables();
    url_encoder_rfc_tables_init();
    card_t c = find_card("Cathar's Crusade");
    char* dstring = display_card(&c);
    printf("%s\n", dstring);
    free(dstring);
    delete_card(c);
    return 0;
}
