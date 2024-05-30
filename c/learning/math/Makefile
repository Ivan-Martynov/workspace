# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = permutations_demo

.PHONY: all
all: $(TARGETS)

permutations_demo: permutations_demo.o permutations.o
	$(LINK_CMD) $^ -o $@ $(LIBS)

permutations_demo.o: permutations_demo.c permutations.h
	$(CC) -c $(CFLAGS) $< -o $@

permutations.o: permutations.c permutations.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm $(OBJ) $(TARGETS)
