CC=g++
OBJ=./*.cpp

CXXFLAGS=-I/usr/include/SDL2 -D_REENTRANT -lSDL2_image -lSDL2_ttf -pthread -lSDL2
OFILES=logger.o


all: ${OBJ} ${OFILES}
	${CC} -o all ${OBJ} ${OFILES} ${CXXFLAGS}


debug.o:
	${CC} -o debug.o -c debug.cpp

release.o:
	${CC} -o release.o -c release.cpp

utils.o:
	${CC} -o utils.o -c lib/mysdl.cpp

logger.o:
	${CC} -o logger.o -c lib/logger.cpp
