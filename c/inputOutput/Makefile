# Makefile

# ------------------------------ VARIABLES ------------------------------ #

# Compiler to use
COMPILER = gcc
# Flags for compiler
CFLAGS   = -Wall -pedantic -std=gnu99
# Define libraries to link into executable
LIBS     = -lncurses
# Flags for debugging (combine multiple flags with spaces)
DFLAGS   = -D__DEBUG_EN -g
# Source files (only .c, no .h)
# SOURCES_GEN = main.c
SOURCES = main.c
# Output file
OUTPUT = main

# ------------------------------ COMMANDS ------------------------------ #

# Build all ("make")
all: build

# Build application "make build"
build:
	$(COMPILER) $(SOURCES) $(CFLAGS) $(LIBS) -o $(OUTPUT)

# Build application with debugging info # "make debug"
debug:
	$(COMPILER) $(SOURCES) $(CFLAGS) $(DFLAGS) $(LIBS) -o $(OUTPUT)

# Remove generated files: "make clean"
clean:
	@if [ $(OUTPUT) ] ; then rm -rf $(OUTPUT) ; fi

