C_COMP = gcc

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
#	 -Weffc++ = warn about violations of the style guidelines from Scott
#	 Meyers' Effective C++ series of books

C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SOURCES := $(wildcard *.c)
TARGET := main.out

debug:
	$(C_COMP) $(SOURCES) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) -o $(TARGET)

release:
	$(C_COMP) $(SOURCES) $(C_FLAGS) $(RELEASE_FLAGS) $(LIBS) -o $(TARGET)

clean:
	@-rm -f $(TARGETS)

distclean: clean
	@-rm -f *~
