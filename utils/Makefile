include ../Makefile.variable

HTTPS_DIR = $(CURDIR)/https_client

SOURCES = arraylist.c cJSON.c error_handler.c urlencode.c net.c dict.c
OBJS = $(SOURCES:.c=.o)

all: $(OBJS)

dict.o: dict.c dict.h arraylist.h error_handler.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

arraylist.o: arraylist.c arraylist.h error_handler.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

net.o: net.c net.h error_handler.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:clean
clean:
	rm -f $(OBJS)