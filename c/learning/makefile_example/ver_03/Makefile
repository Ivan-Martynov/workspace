# In this version we include another makefile. The main makefile pauses and
# waits for the second makefile to complete.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include make_compiler_setup.mk

# List of targets to make.
TARGETS = edit

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all
all: $(TARGETS)

OBJECTS = main.o kbd.o command.o display.o insert.o search.o files.o utils.o

# $@ - target, $^ - all prerequisites without duplicates.
edit : $(OBJECTS)
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

# Include makefile with objects compilation recipies.
include make_objects.mk

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY : clean
# Using '-' character for Makefile to ignore errors.
clean :
	-rm $(OBJECTS) $(TARGETS)