#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scryfall.h"


int main(int argc, char *argv[]) {
    card_t c = find_card("Solitude");
    printf("Card: %s \n", c.name);
    return 0;
}
