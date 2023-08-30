CC = gcc

# -Wshadow and -pedantic flags removed for testing purposes.
CFLAGS = -std=c17 -Wall -Wshadow -pedantic -Wextra -Werror

DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LDFLAGS =

COMPILE_CMD = $(CC) $(CFLAGS) $(DEBUG_FLAGS)
LINK_CMD = $(CC)

TARGETS = edit

.PHONY: all
all: $(TARGETS)

OBJECTS = main.o kbd.o command.o display.o \
	insert.o search.o files.o utils.o

edit : $(OBJECTS)
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

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

#$(OBJECTS) : defs.h
#kbd.o command.o files.o : command.h
#display.o insert.o search.o files.o : buffer.h

.PHONY : clean
clean :
	rm $(OBJECTS) $(TARGETS)