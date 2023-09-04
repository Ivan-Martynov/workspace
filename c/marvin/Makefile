include ../make_compiler_setup.mk

BIN = bin/
OBJ = obj/

TARGETS = 	$(BIN)mem_util_demo\
			$(BIN)sorting_demo

.PHONY: all
all: $(TARGETS)

# Binaries
$(BIN)mem_util_demo: $(OBJ)mem_util_demo.o\
		$(OBJ)memory_blocks_operations.o
	$(LINK_CMD) $^ -o $@

$(BIN)sorting_demo: $(OBJ)sorting_demo.o\
		$(OBJ)sorting_algorithms.o\
		$(OBJ)memory_blocks_operations.o\
		$(OBJ)memory_management_helper.o\
		$(OBJ)comparison_helper.o\
		$(OBJ)timespec_helper.o
	$(LINK_CMD) $^ -o $@

# Objects
$(OBJ)mem_util_demo.o: demos/mem_util_demo.c\
		memory_utilities/memory_blocks_operations.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ)sorting_demo.o: demos/sorting_demo.c\
		algorithms/sorting/sorting_algorithms.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ)sorting_algorithms.o:\
		algorithms/sorting/sorting_algorithms.c\
		algorithms/sorting/sorting_algorithms.h\
		memory_utilities/memory_blocks_operations.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ)memory_blocks_operations.o:\
		memory_utilities/memory_blocks_operations.c\
		memory_utilities/memory_blocks_operations.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ)memory_management_helper.o:\
		memory_utilities/memory_management_helper.c\
		memory_utilities/memory_management_helper.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ)comparison_helper.o:\
		comparison_utilities/comparison_helper.c\
		comparison_utilities/comparison_helper.h
	$(COMPILE_CMD) -c $< -o $@

$(OBJ)timespec_helper.o:\
		date_time_utilities/timespec_helper.c\
		date_time_utilities/timespec_helper.h
	$(COMPILE_CMD) -c $< -o $@

clean:
	rm $(OBJ)*.o $(TARGETS)