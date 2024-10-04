# In this version make deduces the recipies for the object files.
# The only prerequisites are the header files.
# The recipies are organized in a more compact way.

# Specifying explicitly compilation and linking commands.
COMPILE_CMD := $(CC) $(CFLAGS)

# List of targets to make.
TARGETS := edit

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all
all: $(TARGETS)

OBJECTS = main.o kbd.o command.o display.o insert.o search.o files.o utils.o

# $@ - target, $^ - all prerequisites without duplicates.
edit : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Let make deduce the recipies. Might not be very clear but is compact.
$(OBJECTS) : defs.h
kbd.o command.o files.o : command.h
display.o insert.o search.o files.o : buffer.h

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: clean clean_deps clean_all
# Using '-' character for Makefile to ignore errors.
clean: ; -@rm -f $(OBJECTS) $(TARGETS)
clean_deps:
clean_all: clean clean_deps
