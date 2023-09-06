C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I $(INC_DIR)
COMP_CMD = $(C_COMP) $(C_FLAGS)
DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include
# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)

sources = $(wildcard $(SRC_DIR)/*.c)
objects = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(sources))

target = $(BIN_DIR)/main.exe
debug_target = $(BIN_DIR)/debug/main.exe
release_target = $(BIN_DIR)/release/main.exe

.PHONY : all
all : $(target)

# Make a list of object (.o) files from the present source (.c) files.
#objects = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))
#objects = main.o tic_tac_toe.o
#objects = $(OBJ_DIR)/main.o $(OBJ_DIR)/tic_tac_toe.o

debug : $(objects)
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $(debug_target)

release : $(objects)
	$(COMP_CMD) $(RELEASE_FLAGS) $(LIBS) $^ -o $(release_target)

$(target) : $(objects)
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c tic_tac_toe.h
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) -c $< -o $@

#$(OBJ_DIR)/tic_tac_toe.o : $(SRC_DIR)/tic_tac_toe.c tic_tac_toe.h
#	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) -c $< -o $@

#$(OBJ_DIR)/main.o : $(SRC_DIR)/main.c tic_tac_toe.h
#	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) -c $< -o $@

#tic_tac_toe.o : tic_tac_toe.c tic_tac_toe.h
#	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) -c $<

#main.o : main.c tic_tac_toe.h
#	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) -c $<

.PHONY : clean clean_obj

clean : clean_obj
	rm $(target) $(debug_target) $(release_target)

clean_obj :
	rm $(objects)
