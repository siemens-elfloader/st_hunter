

timer = ${PWD}
CC = codeblocks
SOURCE = luatimer.cbp
TARGET = timer
MKDIR = mkdir
BINDIR = ../../build
INSTALL = install -c

all: nsg sg elka

nsg: $(SOURCE)
	$(CC) --rebuild --target=NSG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/nsg/lib"
	$(INSTALL) "$(timer)/exe/timer.so" "$(BINDIR)/nsg/lib/"

sg: $(SOURCE)
	$(CC) --rebuild --target=SG $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/sg/lib"
	$(INSTALL) "$(timer)/exe/timer.so" "$(BINDIR)/sg/lib/"

elka: $(SOURCE)
	$(CC) --rebuild --target=ELKA $(SOURCE)
	$(MKDIR) -p "$(BINDIR)/elka/lib"
	$(INSTALL) "$(timer)/exe/timer.so" "$(BINDIR)/elka/lib/"

