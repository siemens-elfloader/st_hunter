FLAGS = -g

ifeq ($(ARCH), 64)
FLAGS += -m64
else
FLAGS += -m32
endif

CFLAGS += -I./ -c ${FLAGS} #-D__DEBUG__
CPPFLAGS += ${CFLAGS}
CXXFLAGS += ${CFLAGS}

OBJS = Base64.o flap.o httpproxy.o icqkid2.o md5.o snaccache.o  socks4proxy.o \
         socks5proxy.o tnetwork.o const_strings.o miif.o FreiG11.o

all: ${OBJS}
	g++ ${OBJS} ${FLAGS} -Xlinker --no-as-needed -lstdc++ -lpthread -o ig11

clean:
	-rm *.o *~ ig11
