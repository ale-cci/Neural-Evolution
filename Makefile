CC=g++
OBJ=./*.cpp

CXXFLAGS=-I/usr/include/SDL2 -D_REENTRANT -lSDL2_image -lSDL2_ttf -pthread -lSDL2
OFILES=logger.o


all: ${OBJ}
	${CC} -o all ${OBJ} ${OFILES} ${CXXFLAGS}


debug.o:
	${CC} -o debug.o debug.cpp

release.o:
	${CC} -o release.o release.cpp

utils.o:
	${CC} -o utils.o lib/mysdl.cpp

logger.o:
	${CC} -o logger.o lib/logger.cpp
