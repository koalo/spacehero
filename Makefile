CC=gcc 
CFLAGS+=-Wall -Wextra
CFLAGS+=`sdl-config --cflags`
LDFLAGS+=-lm -lGL -lGLU `sdl-config --libs`

#CFLAGS+=-ftree-vectorize
#CFLAGS+=-funroll-all-loops

# Vollbild
#CFLAGS+=-DFULLSCREEN

# langsam aber ansi
#CFLAGS+=-ansi -pedantic

# schnell aber nicht ansi
CFLAGS+=-DFAST -DBETTER --std=gnu99
CFLAGS+=-O3

#CFLAGS+=-march=pentium4 -mfpmath=sse,387 -ffast-math

#CFLAGS+=-pg
#LDFLAGS+=-pg

SRC=displaySpacehero.c displayAbstract.c galaxy.c game.c spacehero.c buttons.c glprint.c levelladen.c
OBJS=$(SRC:.c=.o)

spacehero: $(OBJS)

clean:
	rm -f spacehero $(OBJS)
	rm -f .depend

.depend:
	$(CC) $(CFLAGS) -E -MM $(SRC) > .depend

include .depend
