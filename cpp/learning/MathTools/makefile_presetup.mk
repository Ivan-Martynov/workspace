# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Defining compilers.
CC := gcc
CXX := g++

# Flags for linking and libraries.
LDFLAGS :=
LDLIBS :=

# Set name for the default target to build.
BUILD := debug

# Define directories.
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin/$(BUILD)
DEP_DIR := dep
INC_DIR := include

# Flags used for either target.
compilation_flags.common := -I $(INC_DIR) -Wall -Wshadow -Wpedantic -Wextra \
	-Werror
# Debug specific flags.
compilation_flags.debug := -Og -g -D _DEBUG
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
CFLAGS := -std=c2x ${compilation_flags.common} ${compilation_flags.${BUILD}}

# Replacing default CXXFLAGS with custom ones.
CXXFLAGS := -std=c++23 ${compilation_flags.common} -Weffc++ \
	${compilation_flags.${BUILD}}

# Define commands to build libraries.
STATIC_LIB_CMD := ar rsc
CC_SHARED_LIB_CMD = $(CC) $(CFLAGS) -fPIC -shared
CXX_SHARED_LIB_CMD = $(CXX) $(CXXFLAGS) -fPIC -shared

# Define file extensions.
CC_SRC_EXT := c
CXX_SRC_EXT := cpp
OBJ_EXT := o
DEP_EXT := d
DEP_TEMP_EXT := Td

# For each item in the folder ($1) find items by given pattern ($2).
# - made as plain assignment to be able to expand later.
# - $(wildcard $(1:=/*)) - get all files and subdirectories in the directory.
#	Called for each item, but for files returns nothing.
# - Using call function recursively to check subfolders ($d) using the same
#	pattern ($2).
# - Using filter function to choose items in the folder matching the pattern,
#	(e.g., using pattern *.ext will find all source files and ignore
#	other items).
recursive_wilcard = $(foreach d,$(wildcard $(1:=/*)), \
	$(call recursive_wilcard,$d,$2) $(filter $(subst *,%,$2), $d))

# All source files are in a separate directory.
CC_SOURCES := $(call recursive_wilcard,$(SRC_DIR),*.$(CC_SRC_EXT))
CXX_SOURCES := $(call recursive_wilcard,$(SRC_DIR),*.$(CXX_SRC_EXT))
# Put object files are into a separate directory.
CC_OBJECTS := $(CC_SOURCES:$(SRC_DIR)%.$(CC_SRC_EXT)=$(OBJ_DIR)%.$(OBJ_EXT))
CXX_OBJECTS := $(CXX_SOURCES:$(SRC_DIR)%.$(CXX_SRC_EXT)=$(OBJ_DIR)%.$(OBJ_EXT))
# Put dependency files are into a separate directory.
CC_DEPENDENCIES := \
	$(CC_SOURCES:$(SRC_DIR)%.$(CC_SRC_EXT)=$(DEP_DIR)%.$(DEP_EXT))
CXX_DEPENDENCIES := \
	$(CXX_SOURCES:$(SRC_DIR)%.$(CXX_SRC_EXT)=$(DEP_DIR)%.$(DEP_EXT))
