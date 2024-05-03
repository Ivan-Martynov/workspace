include ../make_compiler_setup.mk

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = \
		  even_fibonacci.exe \
		  colour_output.exe \
		  check_compiler.exe \
		  largest_prime.exe

.PHONY: all
all: $(TARGETS)

check_compiler.exe: check_compiler.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

largest_prime.exe: largest_prime.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS) -lm -pthread

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
