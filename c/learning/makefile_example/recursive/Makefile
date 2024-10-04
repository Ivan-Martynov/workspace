# Calling make files recursively for different folders.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Defining compiler. Exporting to make it availabe for recursive make.
export CC := gcc

# Flags for linking and libraries.
export LDFLAGS :=
export LIBS :=

# Set name for the default target to build.
BUILD := debug

# Flags used for either debug or release.
compilation_flags.common := -std=c2x -Wall -Wshadow -Wpedantic -Wextra -Werror
# Debug specific flags.
compilation_flags.debug := -O0 -g -D _DEBUG
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
export CFLAGS := ${compilation_flags.common} ${compilation_flags.${BUILD}}

# All folder to process.
FOLDERS := ver_01 ver_02 ver_03 ver_04 ver_05 ver_06 ver_07 ver_08 

.PHONY: ${NAMES_TO_BUILD} ${NAMES_TO_CLEAN} ${FOLDERS}

# Add prefix to folder's name for a rule to have different name.
BUILD_PREFIX := build_folder_
NAMES_TO_BUILD := $(patsubst %, ${BUILD_PREFIX}%, ${FOLDERS})

# Add prefix to folder's name for a rule to have different name.
CLEAN_PREFIX := clean_folder_
NAMES_TO_CLEAN := $(patsubst %, ${CLEAN_PREFIX}%, ${FOLDERS})

# Add prefix to folder's name for a rule to have different name.
CLEAN_DEPS_PREFIX := clean_folder_deps_
NAMES_TO_CLEAN_DEPS := $(patsubst %, ${CLEAN_DEPS_PREFIX}%, ${FOLDERS})

# Add prefix to folder's name for a rule to have different name.
CLEAN_ALL_PREFIX := clean_folder_all_
NAMES_TO_CLEAN_ALL := $(patsubst %, ${CLEAN_ALL_PREFIX}%, ${FOLDERS})

#all: ${FOLDERS}

#${FOLDERS}: ; ${MAKE} -C $@

all: ${NAMES_TO_BUILD}

# Remove the prefix to have access to the original folder. Not adding space
# after the comma for the substitution, because that is added to the result.
${NAMES_TO_BUILD}: ; ${MAKE} -C $(patsubst ${BUILD_PREFIX}%,%, $@)

${NAMES_TO_CLEAN}:
	${MAKE} clean -C $(patsubst ${CLEAN_PREFIX}%,%,$@)

${NAMES_TO_CLEAN_DEPS}:
	${MAKE} clean_deps -C $(patsubst ${CLEAN_DEPS_PREFIX}%,%, $@)

${NAMES_TO_CLEAN_ALL}:
	-${MAKE} clean_all -C $(patsubst ${CLEAN_ALL_PREFIX}%,%, $@)

.PHONY: clean clean_deps clean_all

clean: ${NAMES_TO_CLEAN}
clean_deps: ${NAMES_TO_CLEAN_DEPS}
clean_all: ${NAMES_TO_CLEAN_ALL}
	