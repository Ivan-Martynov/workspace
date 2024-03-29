# Flags:
#	 -O0 = reduce compilation time and make debugging produce the expected
#	 results (default, see man g++)
#	 -g = does not affect code generation, only the symbol table and debug
#	 metadata are changed (from stackoverflow)
#	 -Wall = turns on all optional warnings
#	 -Wextra = enables some extra warning flags that are not enabled by -Wall
#	 -Wshadow = warn whenever a local variable or type declaration shadows
#	 another (warns if a local var shadows an explicit typedef, but not if it
#	 shadows a struct/class/enum)

C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c src
vpath %.h include

# SOURCES := $(wildcard src/*.c)
TARGETS := main.out

# $^ - '^' means all unique prerequisites separated by space.
# $@ - '@' means the target.
main.out: main.o env_helper.o
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

# $< - '<' means the first prerequisite.
%.o: %.c %.h
	$(COMP_CMD) -c $<

main.o: main.c
	$(COMP_CMD) -c $<

clean:
	rm -f $(TARGETS) $(wildcard *.o)

distclean: clean
	rm -f  $(wildcard *~)
