

fficore=${PWD}
CC=codeblocks
SOURCE = luafficores.cbp
TARGET = fficore
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib/core"
	$(INSTALL) "$(fficore)/exe/fficore.so" "$(BINDIR)/nsg/lib/core/"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib/core"
	$(INSTALL) "$(fficore)/exe/fficore.so" "$(BINDIR)/sg/lib/core/"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib/core"
	$(INSTALL) "$(fficore)/exe/fficore.so" "$(BINDIR)/elka/lib/core/"

