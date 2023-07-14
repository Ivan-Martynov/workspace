C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGET = main

$(TARGET): main.o size_info.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $(TARGET)

main.o: main.c

size_info.o: size_info.c size_info.h

clean: clean_obj
	rm $(TARGET)

clean_obj:
	rm $(OBJ)
