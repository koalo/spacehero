


CXXFLAGS+=-Wall -Wextra -Wparentheses
CXXFLAGS+=-Weffc++
CXXFLAGS+=`sdl-config --cflags`
CXXFLAGS+=-g
LDFLAGS+=-lm -lGL -lGLU `sdl-config --libs` -lSDL_image -lboost_filesystem-mt

#CFLAGS+=-ftree-vectorize
#CFLAGS+=-funroll-all-loops

# Lokal
CFLAGS+=-DDEV

# Vollbild
#CFLAGS+=-DFULLSCREEN

# langsam aber ansi
#CFLAGS+=-ansi -pedantic

# schnell aber nicht ansi
CXXFLAGS+=-DFAST -DBETTER
CXXFLAGS+=-O3

#CFLAGS+=-march=pentium4 -mfpmath=sse,387 -ffast-math

#CFLAGS+=-pg
#LDFLAGS+=-pg

#SRC=displaySpacehero.c displayAbstract.c galaxy.c game.c spacehero.c buttons.c glprint.c levelladen.c

SRC+=BStatus.cpp 
SRC+=GLdisplay.cpp 
SRC+=Level.cpp
SRC+=Universe.cpp 
SRC+=Spacehero.cpp
SRC+=SpaceDisplay.cpp
SRC+=handleEvents.cpp 
SRC+=intro.cpp
SRC+=output.cpp
SRC+=main.cpp

OBJS=$(SRC:.cpp=.o)

all: spacehero

spacehero: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -f spacehero $(OBJS)
	rm -f .depend

install:
	install -d $(DESTDIR)/usr/games/
	install spacehero $(DESTDIR)/usr/games/
	install -d $(DESTDIR)/usr/share/games/spacehero/level
	install -d $(DESTDIR)/usr/share/games/spacehero/data
	install level/* $(DESTDIR)/usr/share/games/spacehero/level
	install data/* $(DESTDIR)/usr/share/games/spacehero/data

dist:
	 git archive --format=tar --prefix=spacehero/ HEAD | gzip > spacehero.tgz

.depend: $(SRC)
	$(CXX) $(CXXFLAGS) -E -MM $(SRC) > .depend

-include .depend
