CC = gcc

# -Wshadow flag removed for testing purposes.
CFLAGS = -std=c17 -Wall -Wextra -pedantic -Werror

DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LDFLAGS = -lm

COMPILE_CMD = $(CC) $(CFLAGS) $(DEBUG_FLAGS)
LINK_CMD = $(CC)

OBJ = $(wildcard *.o)

TARGETS = listing_13_2.exe

.PHONY: all
all: $(TARGETS)

listing_13_2.exe: listing_13_2.o
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(COMPILE_CMD) -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJ) $(TARGETS)
