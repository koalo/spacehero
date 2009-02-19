
CPPFLAGS+=`sdl-config --cflags`

CXXFLAGS+=-Wall -Wextra -Wparentheses
CXXFLAGS+=-Weffc++
CXXFLAGS+=-g
LIBS+=-lm -lGL -lGLU `sdl-config --libs` -lSDL_image -lboost_filesystem-mt

# Lokal
# z.b. -DDEV
-include Makefile.local

# schnell aber nicht ansi
CXXFLAGS+=-DFAST -DBETTER
CXXFLAGS+=-O2

#CFLAGS+=-pg
#LDFLAGS+=-pg

# eigene OpenGL-Bibliothek
SRC+=GLdisplay.cpp 
SRC+=PictureBook.cpp
SRC+=Illustrator.cpp
SRC+=ButtonMaster.cpp

# SpaceHero
SRC+=Level.cpp
SRC+=Editor.cpp
SRC+=Universe.cpp 
SRC+=Spacehero.cpp
SRC+=SpaceDisplay.cpp
SRC+=intro.cpp
SRC+=output.cpp
SRC+=main.cpp

OBJS=$(SRC:.cpp=.o)

all: spacehero

spacehero: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

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
