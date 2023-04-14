include ../Makefile.variable

HTTPS_DIR = $(CURDIR)/https_client

SOURCES = scryfall.c sqlite_wrapper.c celnet.c telnet.c
OBJS = $(SOURCES:.c=.o)

all: https_make $(OBJS)

https_make:
	$(MAKE) -C $(HTTPS_DIR) mbedtls_make https.o

scryfall.o: scryfall.c scryfall.h https_make ../utils/cJSON.h ../utils/error_handler.h ../utils/urlencode.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

sqlite_wrapper.o: sqlite_wrapper.c sqlite_wrapper.h ../entities/entities.h ../utils/error_handler.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

telnet.o: telnet.c telnet.h celnet.h ../utils/dict.h ../utils/error_handler.h ../utils/net.h ../controllers/parsing.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:https_clean
https_clean:
	$(MAKE) -C $(HTTPS_DIR) clean mbedtls_clean

.PHONY:clean
clean:
	rm -f $(OBJS)

.PHONY:clean_all
clean_all: clean https_clean