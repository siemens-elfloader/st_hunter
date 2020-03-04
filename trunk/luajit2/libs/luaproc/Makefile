

luaproc=${PWD}
CC=codeblocks
SOURCE = luaproc.cbp
TARGET = luaproc
MKDIR=mkdir
BINDIR=../../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib"
	$(INSTALL) "$(luaproc)/exe/luaproc.so" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib"
	$(INSTALL) "$(luaproc)/exe/luaproc.so" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib"
	$(INSTALL) "$(luaproc)/exe/luaproc.so" "$(BINDIR)/elka/lib/"

