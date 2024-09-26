# Defining compiler.
CXX := g++

# Flags for linking and libraries.
LDFLAGS :=
LDLIBS :=

# Set name for the default target to build.
BUILD := debug

# Flags used for either debug or release.
compilation_flags.common := -std=c++23 -Wall -Wshadow -Wpedantic -Wextra -Werror -Weffc++
# Debug specific flags.
compilation_flags.debug := -O0 -g -D _DEBUG
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
CXXFLAGS := ${compilation_flags.common} ${compilation_flags.${BUILD}}

# Specifying explicitly compilation and linking commands.
COMPILE_CMD := $(CXX) $(CFLAGS)
LINK_CMD := $(CXX)
MKLIB_CMD := ar rsc

define simple_source_to_object
$(1).o : $(1).cpp
	@echo "Source to object $$@... "
	$(CXX) -c $(CXXFLAGS) $$< -o $$@
endef

define simple_source_to_binary
$(1): $(1).cpp
	@echo "Building $$@... "
	$(CXX) $(1).cpp $(CXXFLAGS) -o $$@
endef
