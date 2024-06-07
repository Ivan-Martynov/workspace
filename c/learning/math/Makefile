# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = permutations_demo test_lib/permutations_library.a

.PHONY: all
all: $(TARGETS)

permutations_demo: permutations_demo.o test_lib/permutations_library.a
	$(LINK_CMD) $^ -o $@ $(LDLIBS)

test_lib/permutations_library.a: permutations.o
	$(MKLIB_CMD) $@ $^

permutations_demo.o: permutations_demo.c permutations.h
	$(CC) -c $(CFLAGS) $< -o $@

permutations.o: permutations.c permutations.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm $(OBJ) $(TARGETS)
