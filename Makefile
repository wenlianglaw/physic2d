INC=-I/opt/X11/include -I./ -I./p2d_sdl -I./P2D
LIBS=-L/usr/X11/lib -lX11 -lSDL2main -lSDL2
CC=g++
TARGET=physic_2d
CFLAGS= -std=c++17 -Wall 

OBJECTS=$(patsubst %.cpp, %.o, $(shell find . -name "*.cpp" -print))
HEADERS=$(wildcard *.(h|hpp))

all:	build clean

build:	$(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(TARGET) $(INC) $(LIBS)
	

%.o:	%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)


clean:
	find . -name "*.o" -print0| xargs -0 rm
