# This version has explicit recipies for each file.

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
.PHONY: all debug
all: $(TARGETS)

OBJECTS = main.o kbd.o command.o display.o insert.o search.o files.o utils.o

# $@ - target, $^ - all prerequisites without duplicates.
edit : $(OBJECTS)
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

# Spelling out recipies explicitly.
# $< - first prerequisite.
main.o : main.c defs.h
	$(COMPILE_CMD) -c $< -o $@
kbd.o : kbd.c defs.h command.h
	$(COMPILE_CMD) -c $< -o $@
command.o : command.c defs.h command.h
	$(COMPILE_CMD) -c $< -o $@
display.o : display.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@
insert.o : insert.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@
search.o : search.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@
files.o : files.c defs.h buffer.h command.h
	$(COMPILE_CMD) -c $< -o $@
utils.o : utils.c defs.h
	$(COMPILE_CMD) -c $< -o $@

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY : clean
# Using '-' character for Makefile to ignore errors.
clean :
	-rm $(OBJECTS) $(TARGETS)