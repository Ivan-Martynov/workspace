# In this version make deduces the recipies for the object files.
# The only prerequisites are the header files.
# The recipies are organized in a more compact way.

# Defining compiler.
CC = gcc

# Flags for linking and libraries.
LDFLAGS =
LIBS =

# Set name for the default target to build.
BUILD = debug

# Flags used for either debug or release.
compilation_flags.common = -std=c17 -Wall -Wshadow -Wpedantic -Wextra -Werror
# Debug specific flags.
compilation_flags.debug = -O0 -g -D _DEBUG
# Release specific flags.
compilation_flags.release = -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
CFLAGS = ${compilation_flags.common} ${compilation_flags.${BUILD}}

# Specifying explicitly compilation and linking commands.
COMPILE_CMD = $(CC) $(CFLAGS)
LINK_CMD = $(CC)

# List of targets to make.
TARGETS = edit

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all
all: $(TARGETS)

OBJECTS = main.o kbd.o command.o display.o insert.o search.o files.o utils.o

# $@ - target, $^ - all prerequisites without duplicates.
edit : $(OBJECTS)
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

# Let make deduce the recipies. Might not be very clear but is compact.
$(OBJECTS) : defs.h
kbd.o command.o files.o : command.h
display.o insert.o search.o files.o : buffer.h

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY : clean
# Using '-' character for Makefile to ignore errors.
clean :
	-rm $(OBJECTS) $(TARGETS)