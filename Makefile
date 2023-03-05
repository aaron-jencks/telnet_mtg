include ./Makefile.variable

SOURCES = main.c player.c ui.c
IO_SOURCES = io/scryfall.c io/sqlite_wrapper.c
UTILS_SOURCES = utils/arraylist.c utils/cJSON.c utils/error_handler.c utils/urlencode.c
UTILS_OBJS = $(UTILS_SOURCES:.c=.o)
IO_OBJS = $(IO_SOURCES:.c=.o)
MAIN_OBJS = $(SOURCES:.c=.o)
OBJS = $(MAIN_OBJS) $(IO_OBJS) $(UTILS_OBJS)

all: mtg_server

mtg_server: io utils $(MAIN_OBJS)
	$(CC) -o $@ $(HTTPS_DIR)/https.o $(OBJS) $(LDFLAGS) $(LIBS)

io:.
	$(MAKE) -C io

utils:.
	$(MAKE) -C utils

main.o: main.c utils/urlencode.h player.h entities.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

player.o: player.c player.h entities.h utils/error_handler.h
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