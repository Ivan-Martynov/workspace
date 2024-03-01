# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = stddev_demo

.PHONY: all
all: $(TARGETS)

stddev_demo: stddev_demo.o
	$(LINK_CMD) $^ -o $@ $(LIBS) -lm

stddev_demo.o: stddev_demo.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm $(OBJ) $(TARGETS)

