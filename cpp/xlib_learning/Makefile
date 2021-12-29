# TARGET = myfirst mysecond
# CC = g++
# CFLAGS = -c -Wall
# LDFLAGS =
# LIBS: -lX11 -lXinerama
# sources = myfirst.cpp simplewindow.cpp 
# objects = $(sources:.cpp=.o)
# executable = hello

# all: $(sources) $(executable)

# $(executable): $(objects)
# 	$(CC) $(LIBS) $(LDFLAGS) $(objects) -o $@
# clean:
# 	rm -rf *~ $(objects)

simplewindow: simplewindow.o
	g++ simplewindow.o -o simplewindow
simplewindow.o: simplewindow.cpp
	g++ -c simplewindow.cpp -lX11
clean:
	rm -fr simplewindow simplewindow.o
