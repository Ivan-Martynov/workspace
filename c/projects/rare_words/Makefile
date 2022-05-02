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

C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SOURCES := $(wildcard src/*.c)
TARGET := bin/main.out

debug:
	$(C_COMP) $(SOURCES) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) -o $(TARGET)

release:
	$(C_COMP) $(SOURCES) $(C_FLAGS) $(RELEASE_FLAGS) $(LIBS) -o $(TARGET)

main: main.o file_line_reader.o printmode_wide.o rare_word_wide.o string_helper.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) main.o \
	file_line_reader.o printmode_wide.o rare_word_wide.o string_helper.o -o $(TARGET)

file_line_reader.o: src/file_line_reader.c include/file_line_reader.h
	$(C_COMP) $(C_FLAGS) -c src/file_line_reader.c

printmode_wide.o: src/printmode_wide.c include/printmode_wide.h
	$(C_COMP) $(C_FLAGS) -c src/printmode_wide.c

rare_word_wide.o: src/rare_word_wide.c include/rare_word_wide.h
	$(C_COMP) $(C_FLAGS) -c src/rare_word_wide.c

string_helper.o: src/string_helper.c include/string_helper.h
	$(C_COMP) $(C_FLAGS) -c src/string_helper.c

main.o: src/main.c
	$(C_COMP) $(C_FLAGS) -c src/main.c 

clean:
	@-rm -f $(TARGETS) *.o

distclean: clean
	@-rm -f *~

print: src/*.[hc]
	printf $?
	touch $@
