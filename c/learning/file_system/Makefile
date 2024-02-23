# Prevent make from trying to remake the makefile.
#.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

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

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
# TARGETS := list_files.exe
TARGETS := $(BIN_DIR)/$(BUILD)/list_files.exe

.PHONY : all
all : $(TARGETS)

$(TARGETS) : $(OBJECTS)
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

#$(OBJ_DIR)/ID3v2_tag_header_reader.o : $(SRC_DIR)/ID3v2_tag_header_reader.c \
#	$(INC_DIR)/ID3v2_tag_header_reader.h
#	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/list_files.o : $(SRC_DIR)/list_files.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJECTS) : | $(OBJ_DIR)

$(OBJ_DIR) : ; mkdir $(OBJ_DIR)

$(TARGETS) : |$(BIN_DIR)

$(BIN_DIR) : ; mkdir -p $(BIN_DIR)/$(BUILD)

.PHONY : clean
clean : ; rm $(OBJECTS) $(TARGETS)
