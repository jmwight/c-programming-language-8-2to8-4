CC=gcc
ARG=-g3
OBJ=basicio.o myprintf.o

fcopy: fcopy.c ${OBJ} basicio.h
	${CC} ${ARG} -o fcopy ${OBJ} fcopy.c

myprintf.o: myprintf.c
	${CC} ${ARG} -c myprintf.c

basicio.o: basicio.c basicio.h syscalls.h
	${CC} ${ARG} -c basicio.c

.PHONY: clean
clean:
	rm -rf ${OBJ} fcopy
