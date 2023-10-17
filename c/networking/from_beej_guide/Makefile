# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

SRC_DIR := src/
OBJ_DIR := obj/
BIN_DIR := bin/
INC_DIR := include/

CFLAGS += -I $(INC_DIR)
# C_COMP = gcc
# C_FLAGS = -std=c2x -Wall -Wextra -Wshadow -pedantic -Werror -I $(INC_DIR)
# DEBUG_FLAGS = -O0 -g -D _DEBUG
# RELEASE_FLAGS = -O3 -D NDEBUG
# COMP_CMD = $(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS)

LIBS =
#LIBS = -lws2_32 -lwsock32

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)

sources := $(wildcard $(SRC_DIR)*.c)
OBJECTS := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(sources))

OUT_DIR := $(BIN_DIR)$(BUILD)/

TARGET_EXT := exe

SERVER_TARGET := $(OUT_DIR)server.$(TARGET_EXT)
CLIENT_TARGET := $(OUT_DIR)client.$(TARGET_EXT)
SHOW_IP_TARGET := $(OUT_DIR)show_ip.$(TARGET_EXT)

TARGETS := $(SERVER_TARGET) $(CLIENT_TARGET) $(SHOW_IP_TARGET)
#TARGETS := server client show_ip

# release_target := $(BIN_DIR)release/main.out

.PHONY : all
all : $(TARGETS)

debug : all

# release : OUT_DIR = $(BIN_DIR)release
# release : COMP_CMD = $(C_COMP) $(C_FLAGS) $(RELEASE_FLAGS)
# release : all

$(SERVER_TARGET) : $(OBJ_DIR)simple_server.o $(OBJ_DIR)tcp_utils.o $(OBJ_DIR)error_handler.o
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

$(CLIENT_TARGET) : $(OBJ_DIR)simple_client.o $(OBJ_DIR)tcp_utils.o $(OBJ_DIR)error_handler.o
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

$(SHOW_IP_TARGET) : $(OBJ_DIR)show_ip.o $(OBJ_DIR)error_handler.o
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

#	$(COMP_CMD) $^ -o $(OUT_DIR)show_ip.out

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	$(CC) -c $(CFLAGS) $< -o $@
# $(COMP_CMD) -c $< -o $@

$(OBJECTS): | $(OBJ_DIR)
$(TARGETS): | $(BIN_DIR)

$(OBJ_DIR): ; mkdir $(OBJ_DIR)
$(BIN_DIR): ; mkdir $(BIN_DIR)

.PHONY : clean

clean : ; rm $(OBJECTS) $(TARGETS)
