# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

SOURCES := $(wildcard *.cpp)
TARGETS := main
OBJECTS := $(SOURCES:.cpp=.o)

all: $(OBJECTS) main

$(foreach target,$(TARGETS),$(eval $(call simple_source_to_object,$(target))))

main: $(OBJECTS)
	$(LINK_CMD) $^ -o $@ $(LDFLAGS)

clean:
	@-rm -f $(TARGETS) $(OBJECTS)

distclean: clean
	@-rm -f *~
