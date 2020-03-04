SO_OBJS = array.o bytecode.o class.o classfile.o constantpool.o field.o header.o interpreter.o jstring.o kni.o loader.o memory.o method.o native.o print.o thread.o utf8_string.o util.o vm.o

ifeq ($(DEBUG), 1)
CFLAGS = -g -I/usr/include -pthread
CC = gcc
LIBS = -lzzip -lz -lrt -ldl
LDFLAGS = -Wl,-Bsymbolic-functions -L/usr/lib/gcc/i686-linux-gnu/4.6.1 -Xlinker --no-as-needed
LD = gcc
JCP = "../classpath/classpath.jar"
else
CFLAGS = -msoft-float -marm -mlittle-endian -mcpu=arm926ej-s -I/home/barracuda/mysource/sieelf/ep3elf/include -DNEWSGOLD -DELKA
CC = arm-linux-gnueabi-gcc
LIBS = -lzzip-0.36 -lz-1.2.5 -lcrt_helper -lcrt
LDFLAGS = --no-as-needed -L/home/barracuda/mysource/sieelf/ep3elf/libs -s -gc-sections -zmax-page-size=1 -marmelf_linux_eabi --defsym __ex=0
LD = arm-linux-gnueabi-ld
JCP = "0:\\ZBin\\Java.jar"
SO_OBJS += help.o
endif

LIBS += -lc -lm -lgcc

CFLAGS += -fshort-wchar -I. -O2 -DHAVE_CONFIG_H -std=c99 -fshort-enums -pipe -fno-exceptions -fdata-sections -ffunction-sections -nostdlib -DJEL_CLASSPATH_DIR='${JCP}'

FOO_OBJS = libfoo.o

OBJS = main.o

all: clean make_so ${OBJS}
	${LD} -pie ${LDFLAGS} ${LIBS} -L. -ljvm ${OBJS} -o test.elf

make_so: ${SO_OBJS}
	${LD} -shared -fPIC ${LDFLAGS} ${LIBS} ${SO_OBJS} -o libjvm.so

make_foo: ${FOO_OBJS}
	${LD} -shared -fPIC ${LDFLAGS} ${LIBS} -L. -ljvm ${FOO_OBJS} -o libfoo.so

clean:
	-rm *.o test.elf libjvm.so

