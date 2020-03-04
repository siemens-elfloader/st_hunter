

swi=${PWD}
CC=codeblocks
SOURCE = luaswi.cbp
TARGET = swi
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib"
	$(INSTALL) "$(swi)/exe/swi.so" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib"
	$(INSTALL) "$(swi)/exe/swi.so" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib"
	$(INSTALL) "$(swi)/exe/swi.so" "$(BINDIR)/elka/lib/"

