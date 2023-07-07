C_COMP = gcc
C_FLAGS = -std=c11 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGET = main

main: main.o binary_representation.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $(TARGET)

main.o: main.c

binary_representation.o: binary_representation.c binary_representation.h

clean: clean_obj
	rm $(TARGET)

clean_obj:
	rm $(OBJ)
