include ../make_compiler_setup.mk

LDLIBS += -lm

BIN_DIR := bin/
OBJ_DIR := obj/

TARGETS := 	$(BIN_DIR)mem_util_demo\
			$(BIN_DIR)sorting_demo

.PHONY: all
all: $(TARGETS)

# Binaries
$(BIN_DIR)mem_util_demo: $(OBJ_DIR)mem_util_demo.o\
		$(OBJ_DIR)memory_blocks_operations.o
	$(LINK_CMD) $^ -o $@

$(BIN_DIR)sorting_demo: $(OBJ_DIR)sorting_demo.o\
		$(OBJ_DIR)sorting_algorithms.o\
		$(OBJ_DIR)memory_blocks_operations.o\
		$(OBJ_DIR)memory_management_helper.o\
		$(OBJ_DIR)comparison_helper.o\
		$(OBJ_DIR)random_generator_helper.o\
		$(OBJ_DIR)timespec_helper.o
	$(LINK_CMD) $^ -o $@ $(LDLIBS)

# Objects
$(OBJ_DIR)mem_util_demo.o: demos/mem_util_demo.c\
		memory_utilities/memory_blocks_operations.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ_DIR)sorting_demo.o: demos/sorting_demo.c\
		algorithms/sorting/sorting_algorithms.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ_DIR)sorting_algorithms.o:\
		algorithms/sorting/sorting_algorithms.c\
		algorithms/sorting/sorting_algorithms.h\
		memory_utilities/memory_blocks_operations.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ_DIR)memory_blocks_operations.o:\
		memory_utilities/memory_blocks_operations.c\
		memory_utilities/memory_blocks_operations.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ_DIR)memory_management_helper.o:\
		memory_utilities/memory_management_helper.c\
		memory_utilities/memory_management_helper.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ_DIR)comparison_helper.o:\
		numerics/comparison_utilities/comparison_helper.c\
		numerics/comparison_utilities/comparison_helper.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ_DIR)random_generator_helper.o:\
		numerics/pseudo_random_utilities/random_generator_helper.c\
		numerics/pseudo_random_utilities/random_generator_helper.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ_DIR)timespec_helper.o:\
		date_time_utilities/timespec_helper.c\
		date_time_utilities/timespec_helper.h
	$(COMPILE_CMD) -c $< -o $@

# Directories
$(TARGETS): | $(BIN_DIR)

# $(OBJ_DIR): ; mkdir $(OBJ_DIR)
$(BIN_DIR): ; mkdir $(BIN_DIR)

.PHONY: clean

clean: ; rm $(OBJ_DIR)*.o $(TARGETS)
