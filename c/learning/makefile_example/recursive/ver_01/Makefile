# This version has explicit recipies for each file.

# Specifying explicitly compilation and linking commands.
COMPILE_CMD := $(CC) $(CFLAGS)

# List of targets to make.
TARGETS := edit

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all debug
all: $(TARGETS)

OBJECTS := main.o kbd.o command.o display.o insert.o search.o files.o utils.o

# $@ - target, $^ - all prerequisites without duplicates.
edit : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Spelling out recipies explicitly.
# $< - first prerequisite.
main.o : main.c defs.h
	$(COMPILE_CMD) -c $< -o $@
kbd.o : kbd.c defs.h command.h
	$(COMPILE_CMD) -c $< -o $@
command.o : command.c defs.h command.h
	$(COMPILE_CMD) -c $< -o $@
display.o : display.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@
insert.o : insert.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@
search.o : search.c defs.h buffer.h
	$(COMPILE_CMD) -c $< -o $@
files.o : files.c defs.h buffer.h command.h
	$(COMPILE_CMD) -c $< -o $@
utils.o : utils.c defs.h
	$(COMPILE_CMD) -c $< -o $@

.PHONY: clean clean_deps clean_all
clean: ; -@rm -f $(OBJECTS) $(TARGETS)
clean_deps:
clean_all: clean clean_deps
