# Makefile

# ------------------------------ VARIABLES ------------------------------ #

# Compiler to use
COMPILER = gcc
# Flags for compiler
CFLAGS   = -Wall -pedantic -std=gnu99 -lncurses
# Define libraries to link into executable
# LIBS     = -lsctp
# Flags for debugging (combine multiple flags with spaces)
DFLAGS   = -D__DEBUG_EN -g
# Source files (only .c, no .h)
SOURCES_GEN = game.c
# SOURCES_SERVER = serverUDP.c
# SOURCES_CLIENT = clientUDP.c
# Output file
MAINNAME = game
# SERVERNAME = serverUDP
# CLIENTNAME = clientUDP

# ------------------------------ COMMANDS ------------------------------ #

# Build all ("make")
all: build

# Build application "make build"
build:
	$(COMPILER) $(SOURCES_GEN) $(CFLAGS) -o $(MAINNAME)
#	$(COMPILER) $(SOURCES_SERVER) $(CFLAGS) -o $(SERVERNAME)
#	$(COMPILER) $(SOURCES_CLIENT) $(CFLAGS) -o $(CLIENTNAME)

# Build application with debugging info # "make debug"
debug:
	$(COMPILER) $(SOURCES_GEN) $(CFLAGS) $(DFLAGS) -o $(MAINNAME)
#	$(COMPILER) $(SOURCES_SERVER) $(CFLAGS) $(DFLAGS) -o $(SERVERNAME)
#	$(COMPILER) $(SOURCES_CLIENT) $(CFLAGS) $(DFLAGS) -o $(CLIENTNAME)

# Remove generated files: "make clean"
clean:
	@if [ $(MAINNAME) ] ; then rm -rf $(MAINNAME) ; fi
	@if [ $(SERVERNAME) ] ; then rm -rf $(SERVERNAME) ; fi
	@if [ $(CLIENTNAME) ] ; then rm -rf $(CLIENTNAME) ; fi

