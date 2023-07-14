C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = factorial exponentiation

.PHONY: all
all: $(TARGETS)

factorial: factorial.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

factorial.o: factorial.c

exponentiation: exponentiation.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

exponentiation.o: exponentiation.c

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
