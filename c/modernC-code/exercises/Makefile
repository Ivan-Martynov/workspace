C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = UnionFind derivative

.PHONY: all
all: $(TARGETS)

UnionFind: UnionFind.o union_find_table.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

UnionFind.o: UnionFind.c union_find_table.h

union_find_table.o: union_find_table.c union_find_table.h

derivative: derivative.o derivative_fun.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

derivative.o: derivative.c derivative_fun.h

derivative_fun.o: derivative_fun.c derivative_fun.h

size_info.o: size_info.c size_info.h

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
