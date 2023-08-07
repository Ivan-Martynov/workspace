C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = factorial exponentiation accumulation fixed_point continued_fraction

.PHONY: all
all: $(TARGETS)

continued_fraction: continued_fraction.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

continued_fraction.o: continued_fraction.c

fixed_point: fixed_point.o functions.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

fixed_point.o: fixed_point.c functions.h

factorial: factorial.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

factorial.o: factorial.c

exponentiation: exponentiation.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

exponentiation.o: exponentiation.c

accumulation: accumulation.o functions.o
	$(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

accumulation.o: accumulation.c functions.h

functions.o: functions.c functions.h

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
