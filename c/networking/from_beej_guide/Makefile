# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

# Directories.
BIN_DIR := bin/
OBJ_DIR := obj/
SRC_DIR := src/
INC_DIR := include/

CFLAGS += -I $(INC_DIR)
# LDFLAGS += -lws2_32 -lwsock32

SOURCES := $(wildcard $(SRC_DIR)*.c)
OBJECTS := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SOURCES))

OUT_DIR := $(BIN_DIR)$(BUILD)/

TARGET_EXT := exe

SERVER_TARGET := $(OUT_DIR)server.$(TARGET_EXT)
CLIENT_TARGET := $(OUT_DIR)client.$(TARGET_EXT)
SHOW_IP_TARGET := $(OUT_DIR)show_ip.$(TARGET_EXT)

TARGETS := $(SERVER_TARGET) $(CLIENT_TARGET) $(SHOW_IP_TARGET)

.PHONY : all
all : $(TARGETS)

# Targets' recipes.
$(SERVER_TARGET) : $(OBJ_DIR)simple_server.o $(OBJ_DIR)tcp_utils.o \
	$(OBJ_DIR)error_handler.o
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

$(CLIENT_TARGET) : $(OBJ_DIR)simple_client.o $(OBJ_DIR)tcp_utils.o \
	$(OBJ_DIR)error_handler.o
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

$(SHOW_IP_TARGET) : $(OBJ_DIR)show_ip.o $(OBJ_DIR)error_handler.o
	$(LINK_CMD) -o $@ $^ $(LDFLAGS)

# Objects' recipes.
$(OBJ_DIR)error_handler.o : $(SRC_DIR)error_handler.c $(INC_DIR)error_handler.h \
	$(INC_DIR)platform_specific.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)tcp_utils.o : $(SRC_DIR)tcp_utils.c $(INC_DIR)tcp_utils.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)simple_client.o : $(SRC_DIR)simple_client.c $(INC_DIR)net_ids.h \
	$(INC_DIR)platform_specific.h $(INC_DIR)error_handler.h $(INC_DIR)tcp_utils.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)simple_server.o : $(SRC_DIR)simple_server.c $(INC_DIR)net_ids.h \
	$(INC_DIR)platform_specific.h $(INC_DIR)error_handler.h $(INC_DIR)tcp_utils.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)show_ip.o : $(SRC_DIR)show_ip.c $(INC_DIR)error_handler.h \
	$(INC_DIR)platform_specific.h
	$(CC) -c $(CFLAGS) $< -o $@

# Create directories if they don't exist.
$(OBJECTS) : | $(OBJ_DIR)
$(TARGETS) : | $(OUT_DIR)

$(OBJ_DIR) : ; mkdir $(OBJ_DIR)
$(OUT_DIR) : ; mkdir $(BIN_DIR); mkdir $(OUT_DIR)

# Clean up recipe.
.PHONY : clean

clean : ; rm $(OBJECTS) $(TARGETS)
