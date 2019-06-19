CC=g++
OBJ=*.cpp
CXXFLAGS=-I/usr/include/SDL2 -D_REENTRANT -lSDL2_image -lSDL2_ttf -pthread -lSDL2


all: ${OBJ}
	${CC} -o all ${OBJ} ${CXXFLAGS}



