
# Lokal: z.b. -DDEV
-include Makefile.local

CPPFLAGS+=`sdl-config --cflags`

CXXFLAGS+=-Wall -Wextra -Wparentheses
CXXFLAGS+=-Weffc++

LIBS+=-lm -lGL -lGLU `sdl-config --libs` -lSDL_image -lboost_filesystem-mt

# Profiler stuff
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
SRC+=FileManager.cpp
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

INSTALL = install
INSTALL_FILE    = $(INSTALL) -p    -o root -g root  -m  644
INSTALL_PROGRAM = $(INSTALL) -p    -o root -g root  -m  755
INSTALL_SCRIPT  = $(INSTALL) -p    -o root -g root  -m  755
INSTALL_DIR     = $(INSTALL) -p -d -o root -g root  -m  755

DIR_BIN=$(DESTDIR)/usr/games
DIR_SHARED=$(DESTDIR)/usr/share/games/spacehero

install:
	$(INSTALL_DIR) $(DIR_BIN) $(DIR_SHARED)/level $(DIR_SHARED)/data
	$(INSTALL_PROGRAM) spacehero $(DIR_BIN)
	$(INSTALL_FILE) level/* $(DIR_SHARED)/level
	$(INSTALL_FILE) data/* $(DIR_SHARED)/data

dist:
	 git archive --format=tar --prefix=spacehero/ HEAD | gzip > spacehero.tgz

GBPFLAGS+= --git-export-dir=/tmp/gbp
	
debs:
	PBUILDER_OPTS=" --basetgz /var/cache/pbuilder/base-i386.tgz " git-buildpackage $(GBPFLAGS)
	git-buildpackage $(GBPFLAGS) 

.depend: $(SRC)
	$(CXX) $(CXXFLAGS) -E -MM $(SRC) > .depend

-include .depend
