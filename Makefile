CFLAGS=-Wall -std=c99 -fPIC
LDFLAGS=-ldl -shared

.PHONY: all clean

all:	fakemultimon.so

clean:
	rm -f *.o *.so

fakemultimon.so:	fakemultimon.o
	${CC} ${LDFLAGS} -o $@ $^
