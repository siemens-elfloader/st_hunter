

core=${PWD}
CC=codeblocks
SOURCE = luacore.cbp
TARGET = core
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib"
	$(INSTALL) "$(core)/exe/core.so" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib"
	$(INSTALL) "$(core)/exe/core.so" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib"
	$(INSTALL) "$(core)/exe/core.so" "$(BINDIR)/elka/lib/"

