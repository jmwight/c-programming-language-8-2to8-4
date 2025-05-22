CC=gcc
ARG=-g3
OBJ=basicio.o

fcopy: fcopy.c ${OBJ} basicio.h
	${CC} ${ARG} -o fcopy fcopy.c

basicio.o: basicio.c basicio.h syscalls.h
	${CC} ${ARG} -c basicio.c
