include ./Makefile.variable

SOURCES = main.c ui.c
IO_SOURCES = io/scryfall.c io/sqlite_wrapper.c
UTILS_SOURCES = utils/arraylist.c utils/cJSON.c utils/error_handler.c utils/urlencode.c
ENTITY_SOURCES = entities/player.c
ENTITY_OBJS = $(ENTITY_SOURCES:.c=.o)
UTILS_OBJS = $(UTILS_SOURCES:.c=.o)
IO_OBJS = $(IO_SOURCES:.c=.o)
MAIN_OBJS = $(SOURCES:.c=.o)
OBJS = $(MAIN_OBJS) $(IO_OBJS) $(UTILS_OBJS) $(ENTITY_OBJS)

all: mtg_server

mtg_server: io utils entities $(MAIN_OBJS)
	$(CC) -o $@ $(HTTPS_DIR)/https.o $(OBJS) $(LDFLAGS) $(LIBS)

io:.
	$(MAKE) -C io

utils:.
	$(MAKE) -C utils

entities:.
	$(MAKE) -C entities

main.o: main.c utils/urlencode.h entities/player.h entities/entities.h
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