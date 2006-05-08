TARGETS=ixion
OBJECTS=video.o init.o main.o about.o

# Destination root directory for install:
DESTDIR = /
# prefix: install architecture independent files in PREFIX:
PREFIX = usr/local
# exec-prefix: install architecture dependent files in EPREFIX:
EPREFIX = $(PREFIX)
# Path where to Install binaries:
BINPATH = $(DESTDIR)/$(EPREFIX)/bin
# Path where to install images:
PIXMAPSDIR = $(DESTDIR)/$(PREFIX)/share/pixmaps

CC=gcc
CXX=g++
CPPFLAGS=-Wall -O2
#CPPFLAGS=-Wall -g -DDEBUG
CFLAGS=`pkg-config --cflags gstreamer-0.8` `pkg-config --cflags gstreamer-interfaces-0.8`
CXXFLAGS=`wx-config --cppflags` `pkg-config --cflags gtk+-2.0` -DPIXMAPSDIR=\"$(PIXMAPSDIR)\"
LDFLAGS=-lstdc++ `wx-config --libs` `pkg-config --libs gstreamer-0.8` `pkg-config --libs gstreamer-interfaces-0.8`

.PHONY: all clean install

all: $(TARGETS)

ixion: $(OBJECTS)
	g++ -o $@ $(LDFLAGS) $^

about.o: about.cpp about.h ixion.h
main.o: main.cpp ixion.h main.h about.h video.h
init.o: init.cpp ixion.h video.h
video.o: video.c video.h

clean:
	rm -fv $(TARGETS) $(OBJECTS)

install: $(TARGETS)
	install -m755 -s -D ixion $(BINPATH)/ixion
	install -m644 -D ixion-splash.png $(PIXMAPSDIR)/ixion-splash.png

uninstall:
	rm -fv $(BINPATH)/ixion $(PIXMAPSDIR)/ixion-splash.png

