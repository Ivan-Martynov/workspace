
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I $(INC_DIR)
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS = -lws2_32 -lwsock32

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)

sources = $(wildcard $(SRC_DIR)/*.c)
objects = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(sources))

target = $(BIN_DIR)/main.out
debug_target = $(BIN_DIR)/debug/main.out
release_target = $(BIN_DIR)/release/main.out

.PHONY : all
all : $(target)

# Make a list of object (.o) files from the present source (.c) files.

server : $(OBJ_DIR)/simple_server.o
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) \
	$(SRC_DIR)/simple_server.c \
	$(SRC_DIR)/tcp_utils.c \
	$(SRC_DIR)/error_handler.c -o \
	$(BIN_DIR)/debug/server.out

client : $(OBJ_DIR)/simple_client.o
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) \
	$(SRC_DIR)/simple_client.c \
	$(SRC_DIR)/tcp_utils.c \
	$(SRC_DIR)/error_handler.c -o \
	$(BIN_DIR)/debug/client.out

debug : $(objects)
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $(debug_target)

release : $(objects)
	$(COMP_CMD) $(RELEASE_FLAGS) $(LIBS) $^ -o $(release_target)

$(target) : $(objects)
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) -c $< -o $@

.PHONY : clean clean_obj

clean : clean_obj
	rm $(target) $(debug_target) $(release_target)

clean_obj :
	rm $(objects)
