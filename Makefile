CXXFLAGS+=-Wall -Wextra -Wparentheses
CXXFLAGS+=`sdl-config --cflags`
CXXFLAGS+=-g
LDFLAGS+=-lm -lGL -lGLU `sdl-config --libs` -lboost_filesystem-mt

#CFLAGS+=-ftree-vectorize
#CFLAGS+=-funroll-all-loops

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
SRC=spacehero.cpp intro.cpp GLdisplay.cpp Universe.cpp BStatus.cpp handleEvents.cpp displayUniverse.cpp
OBJS=$(SRC:.cpp=.o)

spacehero: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -f spacehero $(OBJS)
	rm -f .depend

dist:
	 git archive --format=tar --prefix=spacehero/ HEAD | gzip > spacehero.tgz

.depend: $(SRC)
	$(CXX) $(CXXFLAGS) -E -MM $(SRC) > .depend

include .depend
