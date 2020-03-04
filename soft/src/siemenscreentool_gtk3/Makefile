PROGRAM = siemens-screenshot-tool
OBJECTS = *.o
SOURCES = *.cpp
PREFIX = /usr/local
CFLAGS = `pkg-config --cflags gtk+-3.0` -lgthread-2.0
LDCONFIG = `pkg-config --libs gtk+-3.0`
INSTALL = install
INSTALL_OPTIONS = -s -m 755 -o 0 -g 0

CXX = g++
.PHONY = all clean install

all: compile

compile: $(OBJS)
	$(CXX) -Wall $(CFLAGS) -o $(PROGRAM) $(SOURCES) $(LDCONFIG)

clean:
	rm -f $(PROGRAM)
	rm -f $(OBJECTS)
	rm -f *~
	
install: all
	$(INSTALL) $(INSTALL_OPTIONS) $(PROGRAM) $(PREFIX)/bin;
	
uninstall:
	rm -f $(PREFIX)/bin/$(PROGRAM)
