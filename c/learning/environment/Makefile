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
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c src
vpath %.h include

SOURCES := $(wildcard src/*.c)
TARGET := main.out

debug:
	$(C_COMP) $(SOURCES) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) -o $(TARGET)

release:
	$(C_COMP) $(SOURCES) $(C_FLAGS) $(RELEASE_FLAGS) $(LIBS) -o $(TARGET)

# $^ - '^' is a special character, meaning all unique prerequisites separated by space.
main: main.o env_helper.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $(TARGET)

env_helper.o: env_helper.c env_helper.h
	$(C_COMP) $(C_FLAGS) -c $<

main.o: main.c
	$(C_COMP) $(C_FLAGS) -c $<

clean:
	@-rm -f $(TARGETS) *.o

distclean: clean
	@-rm -f *~
