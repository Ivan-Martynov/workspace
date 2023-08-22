C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include

DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS = -lm

COMP_CMD = $(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS)

BIN = bin/
OBJ = obj/

TARGETS = 	$(BIN)mem_util_demo\
			$(BIN)sorting_demo

.PHONY: all
all: $(TARGETS)

# Binaries
$(BIN)mem_util_demo: $(OBJ)mem_util_demo.o\
		$(OBJ)memory_blocks_operations.o
	$(COMP_CMD) $^ -o $@

$(BIN)sorting_demo: $(OBJ)sorting_demo.o\
		$(OBJ)sorting_algorithms.o\
		$(OBJ)memory_blocks_operations.o
	$(COMP_CMD) $^ -o $@

# Objects
$(OBJ)mem_util_demo.o: demos/mem_util_demo.c\
		memory_utilities/memory_blocks_operations.h
	$(COMP_CMD) $(DEBUG_FLAGS) -c $< -o $@

$(OBJ)sorting_demo.o: demos/sorting_demo.c\
		algorithms/sorting/sorting_algorithms.h
	$(COMP_CMD) $(DEBUG_FLAGS) -c $< -o $@

$(OBJ)sorting_algorithms.o:\
		algorithms/sorting/sorting_algorithms.c\
		algorithms/sorting/sorting_algorithms.h\
		memory_utilities/memory_blocks_operations.h
	$(COMP_CMD) $(DEBUG_FLAGS) -c $< -o $@

$(OBJ)memory_blocks_operations.o:\
		memory_utilities/memory_blocks_operations.c\
		memory_utilities/memory_blocks_operations.h
	$(COMP_CMD) $(DEBUG_FLAGS) -c $< -o $@

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)*.o