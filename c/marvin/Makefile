C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include

DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS = -lm

COMP_CMD = $(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS)

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = demos/mem_util_demo\
		  demos/sorting_demo

.PHONY: all
all: $(TARGETS)


demos/mem_util_demo: demos/mem_util_demo.o\
		memory_utilities/memory_blocks_operations.o
	$(COMP_CMD) $^ -o $@

demos/mem_util_demo.o: demos/mem_util_demo.c\
	memory_utilities/memory_blocks_operations.h

demos/sorting_demo: demos/sorting_demo.o\
		algorithms/sorting/sorting_algorithms.o\
		memory_utilities/memory_blocks_operations.o
	$(COMP_CMD) $^ -o $@

algorithms/sorting/sorting_algorithms.o:\
	algorithms/sorting/sorting_algorithms.c\
	algorithms/sorting/sorting_algorithms.h\
	memory_utilities/memory_blocks_operations.h


memory_utilities/memory_blocks_operations.o:\
	memory_utilities/memory_blocks_operations.c\
	memory_utilities/memory_blocks_operations.h

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)