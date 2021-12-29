CC = g++
CFLAGS = -c -Wall
LDFLAGS =
sources = main.cpp factorial.cpp hello.cpp listing_simple.cpp
objects = $(sources:.cpp=.o)
executable = hello

all: $(sources) $(executable)

$(executable): $(objects)
	$(CC) $(LDFLAGS) $(objects) -o $@
clean:
	rm -rf *~ *o

