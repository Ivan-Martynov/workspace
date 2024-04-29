include ../make_compiler_setup.mk

BIN_DIR := bin
OBJ_DIR := obj
SRC_DIR := src


C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = largest_prime.exe even_fibonacci.exe colour_output.exe \
	check_compiler.exe

.PHONY: all
all: $(TARGETS)

check_compiler.exe: check_compiler.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

largest_prime.exe: largest_prime.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

even_fibonacci.exe: even_fibonacci.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

colour_output.exe: colour_output.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : show_c_files clean clean_obj

# Add an @ before a command to stop it from being printed.
show_c_files: $(SRC)
	@ls -la $?

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
