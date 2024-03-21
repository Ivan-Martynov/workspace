include ../../make_compiler_setup.mk

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = UnionFind derivative sorting.exe cycle_three_pointers.exe rat_num.exe\
		  binary_representation.exe exercise_39.exe test_time.exe \
		  macros_examples.exe generic_examples.exe

.PHONY: all
all: $(TARGETS)

#listing_13_2.exe: listing_13_2.o
#	$(LINK_CMD) $^ -o $@
#listing_13_2.o: listing_13_2.c

generic_examples.exe: generic_examples.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS) -lm
generic_examples.o: generic_examples.c
	$(CC) -c $(CFLAGS) $< -o $@

macros_examples.exe: macros_examples.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)
macros_examples.o: macros_examples.c
	$(CC) -c $(CFLAGS) $< -o $@

test_time.exe: test_time.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS) -lm
test_time.o: test_time.c
	$(CC) -c $(CFLAGS) $< -o $@

exercise_39.exe: exercise_39.o
	$(LINK_CMD) $^ -o $@
exercise_39.o: exercise_39.c

binary_representation.exe: binary_representation.o
	$(LINK_CMD) $^ -o $@
binary_representation.o: binary_representation.c

rat_num.exe: rat_num.o
	$(LINK_CMD) $^ -o $@

rat_num.o: rat_num.c rat_num.h

cycle_three_pointers.exe: cycle_three_pointers.o
	$(LINK_CMD) $^ -o $@
cycle_three_pointers.o: cycle_three_pointers.c

sorting.exe: sorting.o sorting_algos.o
	$(LINK_CMD) $^ -o $@

sorting.o: sorting.c sorting_algos.h

sorting_algos.o: sorting_algos.c sorting_algos.h

UnionFind: UnionFind.o union_find_table.o
	$(LINK_CMD) $^ -o $@

UnionFind.o: UnionFind.c union_find_table.h

union_find_table.o: union_find_table.c union_find_table.h

derivative: derivative.o derivative_fun.o
	$(LINK_CMD) $^ -o $@ -lm

derivative.o: derivative.c derivative_fun.h

derivative_fun.o: derivative_fun.c derivative_fun.h

size_info.o: size_info.c size_info.h

clean:
	rm $(OBJ) $(TARGETS)
