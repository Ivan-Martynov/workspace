# Add a makefile with compilation flags
include c_comp.mk

objects = tic_tac_toe.o

tic_tac_toe.out: $(objects)
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) -o $@ $^

tic_tac_toe.o : tic_tac_toe.c

.PHONY : clean
clean:
	rm tic_tac_toe.out $(objects)
