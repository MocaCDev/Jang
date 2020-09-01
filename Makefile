SRC = $(wildcard src/*.c)
HDR = $(wildcard src/*.h)
comp = gcc
flags = -Wall -o main.jang.o main.c

.PHONY: Jang
.PHONY: clean

Jang: ${SRC} ${HDR}
	${comp} ${flags}  ${SRC} ${HDR}

clean: ${SRC} ${HDR}
	rm *.o