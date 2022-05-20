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

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
#vpath %.c src
#vpath %.h include

#VPATH = src include
#CPPFLAGS = -I include

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
