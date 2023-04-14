include ./Makefile.variable

SOURCES = main.c ui.c
IO_SOURCES = io/scryfall.c io/sqlite_wrapper.c io/telnet.c io/celnet.c
UTILS_SOURCES = utils/arraylist.c utils/cJSON.c utils/error_handler.c utils/urlencode.c utils/net.c utils/dict.c utils/hashing.c
ENTITY_SOURCES = entities/player.c
CONTROLLER_SOURCES = controllers/parsing.c
ENTITY_OBJS = $(ENTITY_SOURCES:.c=.o)
UTILS_OBJS = $(UTILS_SOURCES:.c=.o)
IO_OBJS = $(IO_SOURCES:.c=.o)
CONTROLLER_OBJS = $(CONTROLLER_SOURCES:.c=.o)
MAIN_OBJS = $(SOURCES:.c=.o)
OBJS = $(MAIN_OBJS) $(IO_OBJS) $(UTILS_OBJS) $(ENTITY_OBJS) $(CONTROLLER_OBJS)

all: mtg_server

mtg_server: bio butils bentities bcontrollers $(MAIN_OBJS)
	$(CC) -o $@ $(HTTPS_DIR)/https.o $(OBJS) $(LDFLAGS) $(LIBS)

bio:.
	$(MAKE) -C io

butils:.
	$(MAKE) -C utils

bentities:.
	$(MAKE) -C entities

bcontrollers:.
	$(MAKE) -C controllers

main.o: main.c utils/urlencode.h entities/player.h entities/entities.h io/telnet.h io/sqlite_wrapper.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

ui.o: ui.c ui.h utils/arraylist.h utils/error_handler.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:https_clean
https_clean:
	$(MAKE) -C $(HTTPS_DIR) clean mbedtls_clean

.PHONY:clean
clean:
	rm -f mtg_server $(OBJS)

.PHONY:clean_db
clean_db:
	rm -f *.db

.PHONY:clean_all
clean_all: clean https_clean clean_db