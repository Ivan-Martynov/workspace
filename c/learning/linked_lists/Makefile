# Add a makefile with compilation flags
include c_comp.mk

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

.DEFAULT_GOAL := $(BINDIR)/doubly_linked_list.out

doubly_linked_list := $(BINDIR)/doubly_linked_list.out

all: $(BINDIR)/doubly_linked_list.out $(BINDIR)/singly_linked_list.out

$(BINDIR)/%.out: $(OBJDIR)/%.o
	$(COMP_CMD) $(DEBUG_FLAGS) $(LIBS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMP_CMD) -c $< -o $@

clean:
	rm -f $(TARGETS) $(BINDIR)/* $(OBJDIR)/*
