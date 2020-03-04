

image=${PWD}
CC=codeblocks
SOURCE = luaimage.cbp
TARGET = image
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	#$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib"
	$(INSTALL) "$(image)/exe/image.lua" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	#$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib"
	$(INSTALL) "$(image)/exe/image.lua" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	#$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib"
	$(INSTALL) "$(image)/exe/image.lua" "$(BINDIR)/elka/lib/"

