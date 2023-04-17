include ../Makefile.variable

SOURCES = parsing.c comm.c
OBJS = $(SOURCES:.c=.o)

all: $(OBJS)

parsing.o: parsing.c parsing.h ../utils/dict.h ../utils/error_handler.h ../utils/hashing.h ../io/celnet.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

comm.o: comm.c comm.h ../utils/error_handler.h
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY:clean
clean:
	rm -f $(OBJS)