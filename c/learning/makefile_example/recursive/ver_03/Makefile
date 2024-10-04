# In this version we include another makefile. The main makefile pauses and
# waits for the second makefile to complete.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Specifying explicitly compilation and linking commands.
COMPILE_CMD := $(CC) $(CFLAGS)

# List of targets to make.
TARGETS := edit

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all
all: $(TARGETS)

OBJECTS := main.o kbd.o command.o display.o insert.o search.o files.o utils.o

# $@ - target, $^ - all prerequisites without duplicates.
edit : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Include makefile with objects compilation recipies.
include make_objects.mk

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: clean clean_deps clean_all
# Using '-' character for Makefile to ignore errors.
clean: ; -@rm -f $(OBJECTS) $(TARGETS)
clean_deps:
clean_all: clean clean_deps
