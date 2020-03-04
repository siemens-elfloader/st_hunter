

draw=${PWD}
CC=codeblocks
SOURCE = luadraw.cbp
TARGET = draw
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib/core"
	$(INSTALL) "$(draw)/exe/draw.so" "$(BINDIR)/nsg/lib/core/"
	$(INSTALL) "$(draw)/exe/draw.lua" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib/core"
	$(INSTALL) "$(draw)/exe/draw.so" "$(BINDIR)/sg/lib/core/"
	$(INSTALL) "$(draw)/exe/draw.lua" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib/core"
	$(INSTALL) "$(draw)/exe/draw.so" "$(BINDIR)/elka/lib/core/"
	$(INSTALL) "$(draw)/exe/draw.lua" "$(BINDIR)/elka/lib/"

