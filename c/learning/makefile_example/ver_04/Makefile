# - Example of using order-only prerequisite to make obj/ directory if it does
# not exist. Consequently, starting file structure reorganization - moving
# object files into a separate directory.
# - Using ; to put the recipe on the same line as the target.
# - Using := to assign variable.

# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include make_compiler_setup.mk

OBJ_DIR := obj/

# List of targets to make.
TARGETS := edit

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all
all: $(TARGETS)

# Define objects as files in the OBJ_DIR directory.
OBJECTS := $(addprefix $(OBJ_DIR),main.o kbd.o command.o display.o insert.o\
	search.o files.o utils.o)

# $@ - target, $^ - all prerequisites without duplicates.
edit : $(OBJECTS)
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)%.o : %.c
	$(COMPILE_CMD) -c $< -o $@

# Make the rule to create the OBJ_DIR directory if needed, without
# rebuilding .o objects if OBJ_DIR directory's timestamp changes.
$(OBJECTS): | $(OBJ_DIR)

# Using ; to put the recipe on the same line as the target.
$(OBJ_DIR): ; mkdir $(OBJ_DIR)

# Include makefile with objects compilation recipies.
# include make_objects.mk

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY : clean

# Using '-' character for Makefile to ignore errors.
# Using ; to put the recipe on the same line as the target.
clean : ; -rm $(OBJECTS) $(TARGETS)
