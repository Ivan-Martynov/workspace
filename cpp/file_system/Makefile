# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Defining compiler. Exporting to make it availabe for recursive make.
CXX := g++

# Set name for the default target to build.
BUILD := debug

# Flags used for either debug or release.
compilation_flags.common := -std=c++23 -Wall -Wshadow -Wpedantic -Wextra \
	-Werror -Weffc++
# Debug specific flags.
compilation_flags.debug := -O0 -g -D _DEBUG
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CXXFLAGS with custom ones.
CXXFLAGS := ${compilation_flags.common} ${compilation_flags.${BUILD}}

STATIC_LIB_CMD := ar rsc

SHARED_LIB_CMD = $(CXX) $(CXXFLAGS) -fPIC -shared

# Define directories.
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin/${BUILD}
DEP_DIR := dep
INC_DIR := include

# Define file extensions.
SRC_EXT := cpp
OBJ_EXT := o
DEP_EXT := d
DEP_TEMP_EXT := Td

# Add include directory to have access to the headers.
# Override allows modify even if the variable has already been defined in the
# command line.
CXXFLAGS += -I $(INC_DIR)

# List of targets to make.
TARGETS := $(BIN_DIR)/show_disk_usage $(BIN_DIR)/batch_rename_files \
	$(BIN_DIR)/organize_files $(BIN_DIR)/mp3_batch_rename_files

# TODO: Introduce a variable to debug the makefile?
DEBUG_MAKEFILE := 0

#include make_config_header.mk
PROJECT_CONFIG_FILE ?=

# For each item in the folder ($1) find items by given pattern ($2).
# - made as plain assignment to be able to expand later.
# - ${wildcard ${1:=/*}} - get all files and subdirectories in the directory.
#	Called for each item, but for files returns nothing.
# - Using call function recursively to check subfolders ($d) using the same
#	pattern ($2).
# - Using filter function to choose items in the folder matching the pattern,
#	(e.g., using pattern *.{SRC_EXT} will find all source files and ignore
#	other items).
recursive_wilcard = ${foreach d,${wildcard ${1:=/*}}, \
	${call recursive_wilcard,$d,$2} ${filter ${subst *,%,$2}, $d}}

# All source files are in a separate directory.
SOURCES := ${call recursive_wilcard,${SRC_DIR},*.${SRC_EXT}}
# Put object files are into a separate directory.
OBJECTS := ${SOURCES:${SRC_DIR}%.${SRC_EXT}=${OBJ_DIR}%.${OBJ_EXT}}
# Put dependency files are into a separate directory.
DEPENDENCIES := ${SOURCES:${SRC_DIR}%.${SRC_EXT}=${DEP_DIR}%.${DEP_EXT}}

.PHONY: all
all: ${TARGETS} ${PROJECT_CONFIG_FILE}

# $@ - target, $^ - all prerequisites without duplicates.
${TARGETS}: | ${BIN_DIR}

$(BIN_DIR)/batch_rename_files: $(OBJ_DIR)/batch_rename_files.o \
	$(OBJ_DIR)/BatchFileRenamer.o $(OBJ_DIR)/Utilities.o \
	$(OBJ_DIR)/FileRenameCommandBase.o $(OBJ_DIR)/FileOverwritePrompt.o \
	$(OBJ_DIR)/CaseModifyCommand.o $(OBJ_DIR)/FileNameValidator.o \
	$(OBJ_DIR)/StringAppendCommand.o $(OBJ_DIR)/StringHelper.o \
	$(OBJ_DIR)/StringReplaceCommand.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/mp3_batch_rename_files: $(OBJ_DIR)/mp3_batch_rename_files.o \
	$(OBJ_DIR)/MP3Tag.o $(OBJ_DIR)/MP3BatchFileRenamer.o \
	$(OBJ_DIR)/BatchFileRenamer.o $(OBJ_DIR)/FileRenameCommandBase.o \
	$(OBJ_DIR)/FileOverwritePrompt.o $(OBJ_DIR)/MP3Tag.o $(OBJ_DIR)/Utilities.o \
	$(OBJ_DIR)/StringAppendCommand.o $(OBJ_DIR)/StringHelper.o \
	$(OBJ_DIR)/CaseModifyCommand.o $(OBJ_DIR)/FileNameValidator.o \
	$(OBJ_DIR)/StringReplaceCommand.o $(OBJ_DIR)/SyncsafeIntConverter.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/organize_files: $(OBJ_DIR)/organize_files.o $(OBJ_DIR)/MP3Tag.o \
	$(OBJ_DIR)/MP3BatchFileRenamer.o $(OBJ_DIR)/BatchFileRenamer.o \
	$(OBJ_DIR)/FileRenameCommandBase.o $(OBJ_DIR)/FileOverwritePrompt.o \
	$(OBJ_DIR)/MP3Tag.o $(OBJ_DIR)/Utilities.o \
	$(OBJ_DIR)/StringAppendCommand.o $(OBJ_DIR)/StringHelper.o \
	$(OBJ_DIR)/CaseModifyCommand.o $(OBJ_DIR)/FileNameValidator.o \
	$(OBJ_DIR)/FileOrganizer.o $(OBJ_DIR)/StringReplaceCommand.o \
	$(OBJ_DIR)/SyncsafeIntConverter.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/show_disk_usage: $(OBJ_DIR)/show_disk_usage.o
	$(CXX) $^ -o $@ $(LDFLAGS)

# Define rules for each object.
define build_object_rule
# Get object's folder to use as an order-only prerequisite.
${eval OBJ_FOLDER := ${dir ${1}}}
# Dependency folder to use as an order-only prerequisite.
${eval DEP_FOLDER := ${OBJ_FOLDER:${OBJ_DIR}%=${DEP_DIR}%}}
# Use variables for source and dependency files for convenience.
${eval SRC_FILE := ${1:${OBJ_DIR}%.${OBJ_EXT}=${SRC_DIR}%.${SRC_EXT}}}
${eval DEP_FILE := ${1:${OBJ_DIR}%.${OBJ_EXT}=${DEP_DIR}%.${DEP_EXT}}}
${eval DEP_TEMP_FILE := ${DEP_FILE:%.${DEP_EXT}=%.${DEP_TEMP_EXT}}}
# Flags for making dependencies:
# https://gcc.gnu.org/onlinedocs/gcc-5.2.0/gcc/Preprocessor-Options.html
# -MT ${1} - Set the name of the target in the dependency file.
# -MMD - generate a dependency output file only with user header files (that is,
#	ignoring system header files) as a side-effect of the compilation process.
# -MP - add a phony target for each dependency file, except the main one. Thus
#	each dependency depends on nothing. Works for cases when header files are
#	removed without updating the Makefile to match.
# -MF ${DEP_TEMP_FILE} - determine a file to write to instead of default.
${eval DEPFLAGS := -MT ${1} -MMD -MP -MF ${DEP_TEMP_FILE}}
# Delete built-in rules.
${1}: ${SRC_FILE}
# Use the custom rule instead.
# - ${DEP_FILE} - use the dependency file as a prerequisite of the target, so
#	that if it's missing, then then target will be rebuilt.
# - | ${OBJ_FOLDER} ${DEP_FOLDER} - dependency directories as an order-only
#	prerequisites of the target.
# - Rename the temporary file with the correct dependency extension and touch to
# update the target's timestamp (that is, make knows it's newer). This trick is
# to prevent problems if a dependency file is corrupted (for example, the build
# was interrupted).
${1}: ${SRC_FILE} ${DEP_FILE} | ${OBJ_FOLDER} ${DEP_FOLDER}
	@echo Building $$@
	@${CXX} ${DEPFLAGS} ${CXXFLAGS} -c -o $$@ $$<
	@mv ${DEP_TEMP_FILE} ${DEP_FILE} && touch $$@
endef

# Build rule for each object.
${foreach FILE_OBJ,${OBJECTS},${eval ${call build_object_rule,${FILE_OBJ}}}}

# Using sort function to retrieve unique folders thus avoiding duplicated rules.
${foreach FOLDER,\
	${sort ${dir ${OBJECTS}}} ${sort ${dir ${DEPENDENCIES}}} ${BIN_DIR},\
	${eval ${FOLDER} : ; @mkdir -p $$@}}

# Each dependency file as a target: make won’t fail if the file doesn’t exist.
${DEPENDENCIES}:
# Include the dependency files with the use of wildcard to avoid
# failing on non-existent files.
-include ${wildcard ${DEPENDENCIES}}

.PHONY: clean clean_deps clean_all
clean: ; @rm -f ${OBJECTS} ${TARGETS}
clean_deps: ; @rm -f ${DEPENDENCIES}
clean_all: clean clean_deps
