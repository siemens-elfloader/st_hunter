

ttf=${PWD}
CC=codeblocks
SOURCE = ttf.lua
TARGET = ttf
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	#$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib"
	$(INSTALL) "$(ttf)/ttf.lua" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	#$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib"
	$(INSTALL) "$(ttf)/ttf.lua" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	#$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib"
	$(INSTALL) "$(ttf)/ttf.lua" "$(BINDIR)/elka/lib/"

