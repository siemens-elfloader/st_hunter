

csm=${PWD}
CC=codeblocks
SOURCE = luacsm.cbp
TARGET = csm
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib"
	$(INSTALL) "$(csm)/exe/csm.so" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib"
	$(INSTALL) "$(csm)/exe/csm.so" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib"
	$(INSTALL) "$(csm)/exe/csm.so" "$(BINDIR)/elka/lib/"

