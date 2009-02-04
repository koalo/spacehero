CC=gcc 
CFLAGS+=-Wall -Wextra
CFLAGS+=`sdl-config --cflags` --save-temps
LDFLAGS+=-lm -lGL -lGLU `sdl-config --libs`

#CFLAGS+=-ftree-vectorize
#CFLAGS+=-funroll-all-loops

# Vollbild
CFLAGS+=-DFULLSCREEN

# langsam aber ansi
#CFLAGS+=-ansi -pedantic

# schnell aber nicht ansi
CFLAGS+=-DFAST -DBETTER --std=gnu99
#CFLAGS+=-march=pentium4 -mfpmath=sse,387 -ffast-math
# laesst Galaxien stocken
#CFLAGS+=-Os

# ca. Geschwindigkeiten:
# ohne alles: 5.95
# mit FAST: 5.00
# mit BETTER: 3.87
# mit FAST und BETTER: 3.16

#CFLAGS+=-pg
#LDFLAGS+=-pg

spacehero: displaySpacehero.o displayAbstract.o galaxy.o game.o spacehero.o buttons.o glprint.o levelladen.o

clean:
	rm -f spacehero displaySpacehero.o displayAbstract.o galaxy.o game.o spacehero.o buttons.o glprint.o levelladen.o
