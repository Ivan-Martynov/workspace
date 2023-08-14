C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include

DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

COMP_CMD = $(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS)

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = count_change_2_19 same_parity_2_20

.PHONY: all
all: $(TARGETS)

same_parity_2_20: same_parity_2_20.o
	$(COMP_CMD) $^ -o $@
same_parity_2_20.o: same_parity_2_20.c

count_change_2_19: count_change_2_19.o
	$(COMP_CMD) $^ -o $@
count_change_2_19.o: count_change_2_19.c

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
