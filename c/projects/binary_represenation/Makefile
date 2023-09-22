CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(CC) $(CFLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGET = main

$(TARGET): main.o binary_representation.o
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $(TARGET)

main.o: main.c

binary_representation.o: binary_representation.c binary_representation.h

clean: clean_obj
	rm $(TARGET)

clean_obj:
	rm $(OBJ)
