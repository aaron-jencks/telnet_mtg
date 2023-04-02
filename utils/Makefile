include ../Makefile.variable

HTTPS_DIR = $(CURDIR)/https_client

SOURCES = arraylist.c cJSON.c error_handler.c urlencode.c
OBJS = $(SOURCES:.c=.o)

all: $(OBJS)

dict.o: dict.c dict.h arraylist.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:clean
clean:
	rm -f $(OBJS)