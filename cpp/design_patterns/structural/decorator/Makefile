# Prevent make from trying to remake the makefile.
#.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../../make_compiler_setup.mk

BIN_DIR := bin
OBJ_DIR := obj
SRC_DIR := src
INC_DIR := include

CFLAGS += -I $(INC_DIR)

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TARGETS := example1.exe example2.exe example3.exe example4.exe

.PHONY : all
all : $(TARGETS)

example1.exe: example1.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

example2.exe: example2.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

example3.exe: example3.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

example4.exe: example4.o
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJECTS) : | $(OBJ_DIR)

$(OBJ_DIR) : ; mkdir $(OBJ_DIR)

$(TARGETS) : |$(BIN_DIR)

$(BIN_DIR) : ; mkdir -p $(BIN_DIR)/$(BUILD)

.PHONY : clean
clean : ; rm $(OBJECTS) $(TARGETS)
