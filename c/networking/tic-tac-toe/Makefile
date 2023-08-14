
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

C_COMP = gcc
C_FLAGS = -std=c2x -Wall -Wextra -Wshadow -pedantic -Werror -I $(INC_DIR)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG

COMP_CMD = $(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS)

LIBS =
#LIBS = -lws2_32 -lwsock32

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)

sources = $(wildcard $(SRC_DIR)/*.c)
objects = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(sources))

OUT_DIR = $(BIN_DIR)/debug

release_target = $(BIN_DIR)/release/main.out

.PHONY : all
all : server client 

debug : all

release : OUT_DIR = $(BIN_DIR)/release
release : COMP_CMD = $(C_COMP) $(C_FLAGS) $(RELEASE_FLAGS)
release : all

server : $(OBJ_DIR)/server.o $(OBJ_DIR)/tcp_utils.o $(OBJ_DIR)/error_handler.o
	$(COMP_CMD) $^ -o $(OUT_DIR)/server.out

client : $(OBJ_DIR)/client.o $(OBJ_DIR)/tcp_utils.o $(OBJ_DIR)/error_handler.o
	$(COMP_CMD) $^ -o $(OUT_DIR)/client.out

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(COMP_CMD) -c $< -o $@

.PHONY : clean clean_bin

clean : clean_bin
	rm $(objects)

clean_bin :
	rm $(BIN_DIR)/debug/* $(BIN_DIR)/release/*