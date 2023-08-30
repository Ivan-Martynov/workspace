CC = gcc

LDFLAGS =
LIBS =

BUILD = debug

compilation_flags.common = -std=c17 -Wall -Wshadow -pedantic -Wextra -Werror
compilation_flags.debug = -O0 -g -D _DEBUG
compilation_flags.release = -O2 -D NDEBUG

CFLAGS = ${compilation_flags.common} ${compilation_flags.${BUILD}}

COMPILE_CMD = $(CC) $(CFLAGS)

LINK_CMD = $(CC)

TARGETS = edit

.PHONY: all
all: $(TARGETS)

OBJECTS = main.o kbd.o command.o display.o \
	insert.o search.o files.o utils.o

include make_objects.mk

edit : $(OBJECTS)
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

.PHONY : clean
clean :
	-rm $(OBJECTS) $(TARGETS)