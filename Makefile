CC=gcc
CPPFLAGS=-I. -Wall
CFLAGS= 
PROG= sokoban
CSRC= main.c sokoban.c ia.c
HSRC= sokoban.h ai.h
OBJ= $(CSRC:.c=.o)

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) $^ -o $@

%.o: %.c %.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	@rm -f *.o *~ $(PROG)
