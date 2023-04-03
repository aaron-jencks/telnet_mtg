include ../Makefile.variable

SOURCES = parsing.c
OBJS = $(SOURCES:.c=.o)

all: $(OBJS)

parsing.o: parsing.c parsing.h ../utils/dict.h ../utils/error_handler.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:clean
clean:
	rm -f $(OBJS)