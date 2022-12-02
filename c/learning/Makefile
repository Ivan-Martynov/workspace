
C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = largest_prime even_fibonacci colour_output
#TARGETS = $(patsubst %.c, %, $(SRC))

.PHONY: all
all: $(TARGETS)

largest_prime: largest_prime.o
	$(C_COMP) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

even_fibonacci: even_fibonacci.o
	$(C_COMP) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

colour_output: colour_output.o
	$(C_COMP) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

#$(OBJ): %.o: %.c
#	$(COMP_CMD) -c $< -o $@

%.o: %.c
	$(COMP_CMD) -c $< -o $@

.PHONY : show_c_files clean clean_obj

# Add an @ before a command to stop it from being printed.
show_c_files: $(SRC)
	@ls -la $?

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
