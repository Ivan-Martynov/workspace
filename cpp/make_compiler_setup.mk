# Defining compiler.
CXX := g++

# Flags for linking and libraries.
LDFLAGS :=
LIBS :=

# Set name for the default target to build.
BUILD := debug

# Flags used for either debug or release.
compilation_flags.common := -std=c++2a -Wall -Wshadow -Wpedantic -Wextra -Werror -Weffc++
# Debug specific flags.
compilation_flags.debug := -O0 -g -D _DEBUG
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
CFLAGS := ${compilation_flags.common} ${compilation_flags.${BUILD}}

# Specifying explicitly compilation and linking commands.
COMPILE_CMD := $(CXX) $(CFLAGS)
LINK_CMD := $(CXX)
