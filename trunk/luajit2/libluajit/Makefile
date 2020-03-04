

libluajit=${PWD}
CC=codeblocks
SOURCE = luajit.cbp
TARGET = main
INSTALL = install -c
BINDIR=../build


all: $(SOURCE)
	$(CC) --build --target=NSG $(SOURCE)
	$(INSTALL) "$(libluajit)/exe/libluajit2.0.0-2.so" "$(BINDIR)/nsg/"
	$(INSTALL) "$(libluajit)/exe/libluajit2.0.0-2.so" "$(BINDIR)/sg/"
	$(INSTALL) "$(libluajit)/exe/libluajit2.0.0-2.so" "$(BINDIR)/elka/"


