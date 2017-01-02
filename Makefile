CC = gcc
FLAGS = -Wall
INCLUDE = -Iinclude
LIB = -lncurses
OBJ = $(wildcard source/*.c)
OUT = PrismEditor

all: build

build: $(OBJ)
	$(CC) $(OBJ) -o $(OUT) $(LIB) $(INCLUDE) $(FLAGS)

run: build
	./$(OUT) prism.sav 
