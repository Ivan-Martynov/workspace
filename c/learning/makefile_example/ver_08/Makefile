# Practing auto generation of dependencies.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include make_compiler_setup.mk

# Define directories.
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin/$(BUILD)
DEP_DIR := $(OBJ_DIR)/dependencies
INC_DIR := include

# Add include directory to have access to the headers.
CFLAGS += -I $(INC_DIR)

# List of targets to make.
TARGETS := $(BIN_DIR)/edit

.PHONY: all
all: $(TARGETS)

# All source files are in a separate directory.
SOURCES := $(wildcard $(SRC_DIR)/*.c)
# Put object files are into a separate directory.
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
# Put dependency files are into a separate directory.
DEPENDENCIES := $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SOURCES))

# $@ - target, $^ - all prerequisites without duplicates.
$(BIN_DIR)/edit : $(OBJECTS) | $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Flags for making dependencies:
# https://gcc.gnu.org/onlinedocs/gcc-5.2.0/gcc/Preprocessor-Options.html
# -MT $@ - Set the name of the target in the dependency file.
# -MMD - generate a dependency output file only with user header files (that is,
#	ignoring system header files) as a side-effect of the compilation process.
# -MP - add a phony target for each dependency file, except the main one. Thus
#	each dependency depends on nothing. Works for cases when header files are
#	removed without updating Makefile to match.
# -MF $(DEP_DIR)/$*.d - determine a file to write to instead of default.
# Using .Td extension for a temporary dependency.
# Recursively expanded variable.
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td

# Build compilation commands to produce dependencies along.
# Recursively expanded variable.
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@ $<

# Rename the temporary file with the correct dependency extension and touch to
# update the timestamp (that is, make knows it's newer). This trick is to
# prevent problems if a dependency file is corrupted (for example, the build
# was interrupted).
# Recursively expanded variable.
POSTCOMPILE = mv $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

# Delete built-in rules.
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
# Use the custom rule instead.
# - $(DEP_DIR)/%.d - use the dependency file as a prerequisite of the target, so
#	that if it’s missing target will be rebuilt.
# - | $(OBJ_DIR) $(DEP_DIR) - dependency directories as an order-only
#	prerequisites of the target, so that they will be created when needed.
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEP_DIR)/%.d | $(OBJ_DIR) $(DEP_DIR)
	$(COMPILE.c)
	$(POSTCOMPILE)

# Create a directory if doesn't exist.
$(OBJ_DIR) : ; mkdir -p $@
$(BIN_DIR) : ; mkdir -p $@
$(DEP_DIR) : ; mkdir -p $@

# Each dependency file as a target: make won’t fail if the file doesn’t exist.
$(DEPENDENCIES):
# Include the dependency files with the use of wildcard to avoid
# failing on non-existent files.
-include $(wildcard $(DEPENDENCIES))

.PHONY: clean clean_deps clean_all

clean: ; -@rm -f $(OBJECTS) $(TARGETS)

clean_deps: ; -@rm -f $(DEPENDENCIES)

clean_all: clean clean_deps
