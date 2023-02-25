MAKE = make

CC = $(CROSS)gcc
LD = $(CROSS)ld
STRIP = $(CROSS)strip

ROOT_DIR = $(CURDIR)
HTTPS_DIR = $(CURDIR)/https_client
MBEDTLS = $(HTTPS_DIR)/mbedtls

CFLAGS = -fPIC -DHAVE_CONFIG_H -D_U_="__attribute__((unused))" -O2
LDFLAGS =

INCLUDES = -I"$(MBEDTLS)/include"

LIBS = "$(MBEDTLS)/library/libmbedx509.a" "$(MBEDTLS)/library/libmbedtls.a" "$(MBEDTLS)/library/libmbedcrypto.a"

SOURCES = main.c scryfall.c
OBJS = $(SOURCES:.c=.o)

all: https_make mtg_server

https_make:
	$(MAKE) -C $(HTTPS_DIR) mbedtls_make https.o

mtg_server: https_make $(OBJS)
	$(CC) -o $@ $(HTTPS_DIR)/https.o $(OBJS) $(LDFLAGS) $(LIBS)

main.o: main.c scryfall.o
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

scryfall.o: scryfall.c scryfall.h https_make
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:https_clean
https_clean:
	$(MAKE) -C $(HTTPS_DIR) clean mbedtls_clean

.PHONY:clean
clean: https_clean
	rm -f mtg_server *.o