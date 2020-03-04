

luajit=${PWD}
CC=codeblocks
SOURCE = lua.cbp
TARGET = main
BINDIR=../build
INSTALL=install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(INSTALL) "$(luajit)/exe/lua.elf" "$(BINDIR)/nsg"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(INSTALL) "$(luajit)/exe/lua_sg.elf" "$(BINDIR)/sg"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(INSTALL) "$(luajit)/exe/lua_el.elf" "$(BINDIR)/elka"

