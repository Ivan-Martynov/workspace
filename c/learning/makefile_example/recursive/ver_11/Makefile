# TODO: Using file function to create a header file.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Set name for the default target to build if it hasn't been set yet.
BUILD ?= debug

# Define directories.
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin/${BUILD}
DEP_DIR := dep
#DEP_DIR := ${OBJ_DIR}/dependencies
INC_DIR := include

# Add include directory to have access to the headers.
# Override allows modify even if the variable has already been defined in the
# command line.
CFLAGS += -I $(INC_DIR)

# List of targets to make.
TARGETS := ${BIN_DIR}/edit

#include make_config_header.mk
PROJECT_CONFIG_FILE ?=

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) \
	$(filter $(subst *,%,$2),$d))

# For each item in the folder ($1) find items by given pattern ($2).
# - made as plain assignment to be able to expand later.
# - ${wildcard ${1:=/*}} - get all files and subdirectories in the directory.
#	Called for each item, but for files returns nothing.
# - Using call function recursively to check subfolders ($d) using the same
#	pattern ($2).
# - Using filter function to choose items in the folder matching the pattern,
#	(e.g., using pattern *.c will find all .c files and ignore other items).
recursive_wilcard = ${foreach d,${wildcard ${1:=/*}}, \
	${call recursive_wilcard,$d,$2} ${filter ${subst *,%,$2}, $d}}
TEST := ${call recursive_wilcard,${SRC_DIR},*.c}

# All source files are in a separate directory.
SOURCES := ${call recursive_wilcard,${SRC_DIR},*.c}
# Put object files are into a separate directory.
OBJECTS := ${SOURCES:${SRC_DIR}%.c=${OBJ_DIR}%.o}
#OBJECTS := ${foreach dir,${SOURCES},${subst ${SRC_DIR},${OBJ_DIR},${dir}}}
# Put dependency files are into a separate directory.
#DEPENDENCIES := ${SOURCES:${SRC_DIR}/%.c=${DEP_DIR}/%.d}
DEPENDENCIES := ${SOURCES:${SRC_DIR}%.c=${DEP_DIR}%.d}
#DEPENDENCIES := ${SOURCES:${SRC_DIR}/%.c=${DEP_DIR}/%.d}

.PHONY: all
#all:
#	@echo ${OBJECTS}
#	@echo ${dir ${OBJECTS}}
#	${TARGETS}
#	${PROJECT_CONFIG_FILE}
all: ${TARGETS}
#	TEMP := ${dir ${OBJECTS}}
#	@echo ${dir ${OBJECTS}}

# Set dependencies for each target.
define Program_template
${1}: ${OBJECTS} | ${BIN_DIR}
endef

# Using eval function to call the template.
${foreach prog,${TARGETS},${eval ${call Program_template,${prog}}}}

# $@ - target, $^ - all prerequisites without duplicates.
${TARGETS}:
	${CC} -o $@ $^ ${LDFLAGS}

# Flags for making dependencies:
# https://gcc.gnu.org/onlinedocs/gcc-5.2.0/gcc/Preprocessor-Options.html
# -MT $@ - Set the name of the target in the dependency file.
# -MMD - generate a dependency output file only with user header files (that is,
#	ignoring system header files) as a side-effect of the compilation process.
# -MP - add a phony target for each dependency file, except the main one. Thus
#	each dependency depends on nothing. Works for cases when header files are
#	removed without updating Makefile to match.
# -MF ${DEP_DIR}/$*.d - determine a file to write to instead of default.
# Using .Td extension for a temporary dependency.
# Recursively expanded variable.
DEPFLAGS = -MT $@ -MMD -MP -MF ${DEP_DIR}/$*.Td

# Build compilation commands to produce dependencies along.
# Recursively expanded variable.
COMPILE.c = ${CC} ${DEPFLAGS} ${CFLAGS} -c -o $@ $<

# Rename the temporary file with the correct dependency extension and touch to
# update the timestamp (that is, make knows it's newer). This trick is to
# prevent problems if a dependency file is corrupted (for example, the build
# was interrupted).
# Recursively expanded variable.
POSTCOMPILE = mv ${DEP_DIR}/$*.Td ${DEP_DIR}/$*.d && touch $@

# Delete built-in rules.
${OBJ_DIR}%.o : ${SRC_DIR}%.c
# Use the custom rule instead.
# - ${DEP_DIR}/%.d - use the dependency file as a prerequisite of the target, so
#	that if it’s missing target will be rebuilt.
# - | ${OBJ_DIR} ${DEP_DIR} - dependency directories as an order-only
#	prerequisites of the target, so that they will be created when needed.
${OBJ_DIR}%.o : ${SRC_DIR}%.c ${DEP_DIR}%.d | ${OBJ_DIR} ${DEP_DIR}
	@echo $< $@
	${COMPILE.c}
	${POSTCOMPILE}

${OBJECTS} : ${SOURCES} ${DEPENDENCIES} | ${OBJ_DIR} ${DEP_DIR}
	@echo $< $@
	${COMPILE.c}
	${POSTCOMPILE}


# Set dependencies for each target.
#define Program_template
#${1}: ${OBJECTS} | ${BIN_DIR}
#endef

# Using eval function to call the template.
#${foreach prog,${TARGETS},${eval ${call Program_template,${prog}}}}

# Set dependencies for each target.
define folder_template
@echo calling template
${1}: | ${dir ${1}}
endef

${foreach dir,${DIRS},${call mkdir -p ${dir}}}
#${foreach prog,${TARGETS},${eval ${call Program_template,${prog}}}}



# Create a directory if doesn't exist.
#${OBJ_DIR} : ; mkdir -p $@
#${BIN_DIR} : ; mkdir -p $@
#${DEP_DIR} : ; mkdir -p $@

# Each dependency file as a target: make won’t fail if the file doesn’t exist.
${DEPENDENCIES}:
# Include the dependency files with the use of wildcard to avoid
# failing on non-existent files.
-include ${wildcard ${DEPENDENCIES}}

.PHONY: clean clean_deps clean_all
clean: ; @rm -f ${OBJECTS} ${TARGETS}
clean_deps: ; @rm -f ${DEPENDENCIES}
clean_all: clean clean_deps
