# Calling make files recursively for different folders.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Defining compiler. Exporting to make it availabe for recursive make.
export CC := gcc

# Flags for linking and libraries.
export LDFLAGS :=
export LIBS :=

# Set name for the default target to build.
export BUILD := debug

# Flags used for either debug or release.
compilation_flags.common := -std=c2x -Wall -Wshadow -Wpedantic -Wextra -Werror
# Debug specific flags.
compilation_flags.debug := -O0 -g -D _DEBUG
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
export CFLAGS := ${compilation_flags.common} ${compilation_flags.${BUILD}}

# All folder to process.
#FOLDERS := ver_01 ver_02 ver_03 ver_04 ver_05 ver_06 ver_07 ver_08  ver_09
FOLDERS := ${patsubst %/,%,${wildcard */}}

.PHONY: ${NAMES_TO_BUILD} ${NAMES_TO_CLEAN} ${NAMES_TO_CLEAN_DEPS} \
	${NAMES_TO_CLEAN_ALL}
# ${FOLDERS}

# Add prefix to folder's name for a rule to have different name.
BUILD_PREFIX := build_folder_
CLEAN_PREFIX := clean_folder_
CLEAN_DEPS_PREFIX := clean_folder_deps_
CLEAN_ALL_PREFIX := clean_folder_all_

#NAMES_TO_BUILD := $(patsubst %, ${BUILD_PREFIX}%, ${FOLDERS})
# Substituting using references.
NAMES_TO_BUILD := ${FOLDERS:%=${BUILD_PREFIX}%}
# Substituting using patsubst.
NAMES_TO_CLEAN := ${patsubst %, ${CLEAN_PREFIX}%, ${FOLDERS}}
# Adding a prefix.
NAMES_TO_CLEAN_DEPS := $(addprefix ${CLEAN_DEPS_PREFIX}, ${FOLDERS})
# Using foreach.
NAMES_TO_CLEAN_ALL := ${foreach dir,${FOLDERS},${CLEAN_ALL_PREFIX}${dir}}

#F := ${patsubst %/,%,${wildcard ver_07/*/}}
TEST := ${foreach dir,${FOLDERS},TEST_${dir}}

#all: ${FOLDERS}

#${FOLDERS}: ; ${MAKE} -C $@

all: ${NAMES_TO_BUILD}
#all: ${TEST}

# Define the underlying operating system.
S_NAME :=
ifeq (${OS},Windows_NT)
	S_NAME += windows_
	ifeq (${PROCESSOR_ARCHITEW6432},AMD64)
		S_NAME += AMD64
	else ifeq (${PROCESSOR_ARCHITECTURE},AMD64)
		S_NAME += AMD64
	else ifeq (${PROCESSOR_ARCHITECTURE},x86)
		S_NAME += IA32
	endif
else
	UNAME_S := ${shell uname -s}
	ifeq ($(UNAME_S),Linux)
        S_NAME += LINUX
    else ifeq ($(UNAME_S),Darwin)
        S_NAME += OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        S_NAME += AMD64
	else ifneq ($(filter %86,$(UNAME_P)),)
        S_NAME += IA32
    else ifneq ($(filter arm%,$(UNAME_P)),)
        S_NAME += ARM
    endif
endif

${TEST}:
	@echo $@
#	@echo ${S_NAME}
#@echo ${notdir ${wildcard ${CURDIR}/*}}

# Remove the prefix to have access to the original folder. Not adding space
# after the comma for the substitution, because that is added to the result.
${NAMES_TO_BUILD}:
	${MAKE} -C ${@:${BUILD_PREFIX}%=%}
#	${MAKE} -C $(patsubst ${BUILD_PREFIX}%,%, $@)

${NAMES_TO_CLEAN}:
	${MAKE} clean -C $(patsubst ${CLEAN_PREFIX}%,%,$@)

${NAMES_TO_CLEAN_DEPS}:
	${MAKE} clean_deps -C $(patsubst ${CLEAN_DEPS_PREFIX}%,%,$@)

${NAMES_TO_CLEAN_ALL}:
	${MAKE} clean_all -C $(patsubst ${CLEAN_ALL_PREFIX}%,%,$@)

.PHONY: clean clean_deps clean_all

clean: ${NAMES_TO_CLEAN}
clean_deps: ${NAMES_TO_CLEAN_DEPS}
clean_all: ${NAMES_TO_CLEAN_ALL}
	