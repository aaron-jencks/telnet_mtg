include ../Makefile.variable

HTTPS_DIR = $(CURDIR)/https_client

SOURCES = arraylist.c cJSON.c error_handler.c urlencode.c
OBJS = $(SOURCES:.c=.o)

all: $(OBJS)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:clean
clean:
	rm -f $(OBJS)

.PHONY:clean_all
clean_all: clean https_clean