SRC = $(wildcard src/*.c)
HDR = $(wildcard src/*.h)
comp = gcc
flags = -Wall -o main.jang.o main.c
VALGRIND = valgrind ./main.jang.o
ALL_JANG_FILES = $(wildcard */*.jang)

.PHONY: Jang
.PHONY: clean
.PHONY: Memory_Check

Jang: ${SRC} ${HDR}
	${comp} ${flags}  ${SRC} ${HDR}

Memory_Check: ${SRC}
	${VALGRIND}

clean: ${SRC} ${HDR}
	rm *.o