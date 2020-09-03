SRC = $(wildcard src/*.c)
HDR = $(wildcard src/*.h)
comp = gcc
flags = -Wall -o main.jang.o main.c
VALGRIND = valgrind ./main.jang.o

.PHONY: Jang
.PHONY: clean
.PHONY: Memory_Check

JangFileToCheck = null

Jang: ${SRC} ${HDR}
	${comp} ${flags}  ${SRC} ${HDR}

Memory_Check: ${C_SRC}
	${VALGRIND} ${JangFileToCheck}

clean: ${SRC} ${HDR}
	rm *.o