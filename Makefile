CC=g++
OBJS=*.cpp
CFLAGS=-w -std=c++11
LFLAGS=-lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network
OUT=citybuild

all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(OUT)
