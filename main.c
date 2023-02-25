#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scryfall.h"


int main(int argc, char *argv[]) {
    card_t c = find_card("Solitude");
    char* dstring = display_card(&c);
    printf("%s\n", dstring);
    free(dstring);
    delete_card(c);
    return 0;
}
