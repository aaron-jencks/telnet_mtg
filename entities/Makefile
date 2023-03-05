include ../Makefile.variable

SOURCES = player.c
OBJS = $(SOURCES:.c=.o)

all: $(OBJS)

player.o: player.c player.h entities.h ../utils/error_handler.h ../io/sqlite_wrapper.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:clean
clean:
	rm -f $(OBJS)