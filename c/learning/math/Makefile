C_COMP = gcc
C_FLAGS = -std=c17 -Wall -Wextra -Wshadow -pedantic -Werror -I include

DEBUG_FLAGS = -O0 -g -D _DEBUG
RELEASE_FLAGS = -O3 -D NDEBUG
LIBS =

COMP_CMD = $(C_COMP) $(C_FLAGS) $(DEBUG_FLAGS) $(LIBS)

SRC = $(wildcard *.c)
OBJ = $(wildcard *.o)

TARGETS = factorial exponentiation accumulation continued_fraction\
	deriv newton_method interval_arithmetic_demo vec_demo list_operations_demo
.PHONY: all
all: $(TARGETS)

list_operations_demo: list_operations_demo.o data_structures/list_operations.o\
	data_structures/vec_d.o data_structures/linked_list_d.o functions.o
	$(COMP_CMD) $^ -o $@
list_operations_demo.o: list_operations_demo.c data_structures/list_operations.h\
	functions.h data_structures/linked_list_d.h

data_structures/list_operations.o: data_structures/list_operations.c\
	data_structures/list_operations.h data_structures/vec_d.h

data_structures/linked_list_d.o: data_structures/linked_list_d.c\
	data_structures/linked_list_d.h

vec_demo: vec_demo.o data_structures/vec_d.o
	$(COMP_CMD) $^ -o $@
vec_demo.o: vec_demo.c data_structures/vec_d.h

data_structures/vec_d.o: data_structures/vec_d.c data_structures/vec_d.h

interval_arithmetic_demo: interval_arithmetic_demo.o interval_arithmetic.o
	$(COMP_CMD) $^ -o $@
interval_arithmetic_demo.o: interval_arithmetic_demo.c interval_arithmetic.h

newton_method: newton_method.o fixed_point.o functions.o rat_num.o
	$(COMP_CMD) $^ -o $@
newton_method.o: newton_method.c fixed_point.h functions.h rat_num.h

deriv: deriv.o functions.o
	$(COMP_CMD) $^ -o $@
deriv.o: deriv.c functions.h

continued_fraction: continued_fraction.o
	$(COMP_CMD) $^ -o $@

continued_fraction.o: continued_fraction.c

fixed_point.o: fixed_point.c fixed_point.h functions.h

factorial: factorial.o
	$(COMP_CMD) $^ -o $@

factorial.o: factorial.c

exponentiation: exponentiation.o
	$(COMP_CMD) $^ -o $@

exponentiation.o: exponentiation.c

accumulation: accumulation.o functions.o
	$(COMP_CMD) $^ -o $@

accumulation.o: accumulation.c functions.h

functions.o: functions.c functions.h

rat_num.o: rat_num.c rat_num.h

interval_arithmetic.o: interval_arithmetic.c interval_arithmetic.h

clean: clean_obj
	rm $(TARGETS)

clean_obj:
	rm $(OBJ)
