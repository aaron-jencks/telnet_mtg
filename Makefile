MAKE = make

CC = $(CROSS)gcc
LD = $(CROSS)ld
STRIP = $(CROSS)strip

ROOT_DIR = $(CURDIR)
HTTPS_DIR = $(CURDIR)/https_client
MBEDTLS = $(HTTPS_DIR)/mbedtls

DEBUG = -g
CFLAGS = $(DEBUG) -fPIC -DHAVE_CONFIG_H -D_U_="__attribute__((unused))" -O2
LDFLAGS = -pthread -lsqlite3

INCLUDES = -I"$(MBEDTLS)/include"

LIBS = "$(MBEDTLS)/library/libmbedx509.a" "$(MBEDTLS)/library/libmbedtls.a" "$(MBEDTLS)/library/libmbedcrypto.a"

SOURCES = main.c scryfall.c cJSON.c urlencode.c error_handler.c sqlite_wrapper.c
OBJS = $(SOURCES:.c=.o)

all: https_make mtg_server

https_make:
	$(MAKE) -C $(HTTPS_DIR) mbedtls_make https.o

mtg_server: https_make $(OBJS)
	$(CC) -o $@ $(HTTPS_DIR)/https.o $(OBJS) $(LDFLAGS) $(LIBS)

main.o: main.c scryfall.o urlencode.o sqlite_wrapper.o
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

scryfall.o: scryfall.c scryfall.h https_make cJSON.o urlencode.o error_handler.o
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

sqlite_wrapper.o: sqlite_wrapper.c sqlite_wrapper.h entities.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:https_clean
https_clean:
	$(MAKE) -C $(HTTPS_DIR) clean mbedtls_clean

.PHONY:clean
clean:
	rm -f mtg_server *.o

.PHONY:clean_db
clean_db:
	rm -f *.db

.PHONY:clean_all
clean_all: clean https_clean clean_db