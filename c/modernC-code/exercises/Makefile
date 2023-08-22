C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

COMP_CMD = $(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS)

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = UnionFind derivative sorting.exe cycle_three_pointers.exe rat_num.exe

.PHONY: all
all: $(TARGETS)

rat_num.exe: rat_num.o
	$(COMP_CMD) $^ -o $@
rat_num.o: rat_num.c rat_num.h

cycle_three_pointers.exe: cycle_three_pointers.o
	$(COMP_CMD) $^ -o $@
cycle_three_pointers.o: cycle_three_pointers.c

sorting.exe: sorting.o sorting_algos.o
	$(COMP_CMD) $^ -o $@
sorting.o: sorting.c sorting_algos.h

sorting_algos.o: sorting_algos.c sorting_algos.h

UnionFind: UnionFind.o union_find_table.o
	$(COMP_CMD) $^ -o $@

UnionFind.o: UnionFind.c union_find_table.h

union_find_table.o: union_find_table.c union_find_table.h

derivative: derivative.o derivative_fun.o
	$(COMP_CMD) $^ -o $@

derivative.o: derivative.c derivative_fun.h

derivative_fun.o: derivative_fun.c derivative_fun.h

size_info.o: size_info.c size_info.h

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
