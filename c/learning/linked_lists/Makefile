# Add a makefile with compilation flags
include c_comp.mk

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

#.DEFAULT_GOAL := $(BINDIR)/doubly_linked_list.out

#doubly_linked_list := $(BINDIR)/doubly_linked_list.out

all: $(BINDIR)/doubly_linked_list.out $(BINDIR)/singly_linked_list.out
.PHONY : all

$(BINDIR)/doubly_linked_list.out: $(OBJDIR)/doubly_linked_list.o
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

$(BINDIR)/singly_linked_list.out: $(OBJDIR)/singly_linked_list.o
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

#$(BINDIR)/%.out: $(OBJDIR)/%.o
#	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMP_CMD) -c $< -o $@

.PHONY : clean clean_obj clean_bin

clean: clean_obj
	rm $(BINDIR)/*

clean_obj :
	rm $(OBJDIR)/*.o