# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = factorial exponentiation accumulation continued_fraction\
	deriv newton_method interval_arithmetic_demo vec_demo list_operations_demo\
	tree_operations_demo float_demo

.PHONY: all
all: $(TARGETS)

float_demo: float_demo.o binary_float.o
	$(LINK_CMD) $^ -o $@ $(LIBS) -lm

tree_operations_demo: tree_operations_demo.o data_structures/tree_operations.o\
	data_structures/binary_tree_d.o
	$(LINK_CMD) $^ -o $@ $(LIBS)

list_operations_demo: list_operations_demo.o data_structures/list_operations.o\
	data_structures/vec_d.o data_structures/linked_list_d.o functions.o\
	data_structures/list_of_lists_d.o
	$(LINK_CMD) $^ -o $@ $(LIBS)

float_demo.o: float_demo.c binary_float.h

binary_float.o: binary_float.c binary_float.h

list_operations_demo.o: list_operations_demo.c data_structures/list_operations.h\
	functions.h data_structures/linked_list_d.h data_structures/list_of_lists_d.h

tree_operations_demo.o: tree_operations_demo.c data_structures/tree_operations.h\
	data_structures/binary_tree_d.h

data_structures/tree_operations.o: data_structures/tree_operations.c\
	data_structures/tree_operations.h data_structures/binary_tree_d.h

data_structures/binary_tree_d.o: data_structures/binary_tree_d.c\
	data_structures/binary_tree_d.h

data_structures/list_operations.o: data_structures/list_operations.c\
	data_structures/list_operations.h data_structures/vec_d.h\
	data_structures/list_of_lists_d.h

data_structures/list_of_lists_d.o: data_structures/list_of_lists_d.c\
	data_structures/list_of_lists_d.h

data_structures/linked_list_d.o: data_structures/linked_list_d.c\
	data_structures/linked_list_d.h

vec_demo: vec_demo.o data_structures/vec_d.o
	$(LINK_CMD) $^ -o $@ $(LIBS)
vec_demo.o: vec_demo.c data_structures/vec_d.h

data_structures/vec_d.o: data_structures/vec_d.c data_structures/vec_d.h

interval_arithmetic_demo: interval_arithmetic_demo.o interval_arithmetic.o
	$(LINK_CMD) $^ -o $@ $(LIBS) -lm
interval_arithmetic_demo.o: interval_arithmetic_demo.c interval_arithmetic.h

newton_method: newton_method.o fixed_point.o functions.o rat_num.o
	$(LINK_CMD) $^ -o $@ $(LIBS) -lm
newton_method.o: newton_method.c fixed_point.h functions.h rat_num.h

deriv: deriv.o functions.o
	$(LINK_CMD) $^ -o $@ $(LIBS)
deriv.o: deriv.c functions.h

continued_fraction: continued_fraction.o
	$(LINK_CMD) $^ -o $@ $(LIBS)

continued_fraction.o: continued_fraction.c

fixed_point.o: fixed_point.c fixed_point.h functions.h

factorial: factorial.o
	$(LINK_CMD) $^ -o $@ $(LIBS)

factorial.o: factorial.c

exponentiation: exponentiation.o
	$(LINK_CMD) $^ -o $@ $(LIBS)

exponentiation.o: exponentiation.c

accumulation: accumulation.o functions.o
	$(LINK_CMD) $^ -o $@ $(LIBS)

accumulation.o: accumulation.c functions.h

functions.o: functions.c functions.h

rat_num.o: rat_num.c rat_num.h

interval_arithmetic.o: interval_arithmetic.c interval_arithmetic.h

clean:
	-rm $(OBJ) $(TARGETS)
