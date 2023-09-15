# - Example of using order-only prerequisite to make obj/ directory if it does
# not exist. Consequently, starting file structure reorganization - moving
# object files into a separate directory.
# - Using ; to put the recipe on the same line as the target.
# - Using := to assign variable.
# - Using wildcard and patsubst to build names for object files.
# - Placing the target into bin/ directory.
# - Generating prerequisites automatically.
#  TODO: Generating prerequisites automatically not working yet.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include make_compiler_setup.mk

BIN_DIR := bin/
OBJ_DIR := obj/

# Make objects' names in the OBJ_DIR directory from source file names.
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, $(OBJ_DIR)%.o, $(SOURCES))

# List of targets to make.
TARGETS := $(BIN_DIR)edit
#TARGETS := $(addprefix $(BIN_DIR), edit)

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all
all: $(TARGETS)

# $@ - target, $^ - all prerequisites without duplicates.
$(BIN_DIR)edit : $(OBJECTS)
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

# Generaing .d files.
# %.d : %.c
# 	@set -e; rm -f $@; \
# 	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
# 	sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)\1.o : ,g' < $@.$$$$ > $@; \
# 	rm -f $@.$$$$

make.dependencies: $(SOURCES)
	$(CC) -MM $(CFLAGS) $(SOURCES) > make.dependencies


-include make.dependencies

#$(OBJ_DIR)%.o : %.c
#	$(CC) -c $(CFLAGS) $< -o $@

# Make the rule to create the OBJ_DIR directory if needed, without
# rebuilding .o objects if OBJ_DIR directory's timestamp changes.
$(OBJECTS): defs.h | $(OBJ_DIR)
$(TARGETS): | $(BIN_DIR)

# Using ; to put the recipe on the same line as the target.
$(OBJ_DIR): ; mkdir $(OBJ_DIR)
$(BIN_DIR): ; mkdir $(BIN_DIR)

# Include makefile with objects compilation recipies.
# include make_objects.mk

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY : clean

# Using '-' character for Makefile to ignore errors.
# Using ; to put the recipe on the same line as the target.
clean : ; -rm $(OBJECTS) $(TARGETS) $(wildcard *.d)
