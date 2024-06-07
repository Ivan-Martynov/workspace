# Defining compiler.
CC := gcc

# Flags for linking and libraries.
LDFLAGS :=
LDLIBS :=

# Set name for the default target to build.
BUILD := debug

# Flags used for either debug or release.
compilation_flags.common := -std=c2x -Wall -Wshadow -Wpedantic -Wextra -Werror
# Debug specific flags.
compilation_flags.debug := -Og -g -D _DEBUG
:q
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
CFLAGS := ${compilation_flags.common} ${compilation_flags.${BUILD}}

# Specifying explicitly compilation and linking commands.
COMPILE_CMD := $(CC) $(CFLAGS)
LINK_CMD := $(CC)
MKLIB_CMD := ar rsc
