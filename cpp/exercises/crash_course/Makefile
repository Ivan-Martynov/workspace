# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../../make_compiler_setup.mk

SOURCES := $(wildcard *.cpp)
TARGETS := $(SOURCES:.cpp=)
OBJECTS := $(SOURCES:.cpp=.o)

all: $(TARGETS)

#$(foreach target,$(TARGETS),$(eval $(call simple_source_to_object,$(target))))
$(foreach target,$(TARGETS),$(eval $(call simple_source_to_binary,$(target))))

clean:
	@-rm -f $(TARGETS) $(OBJECTS)

distclean: clean
	@-rm -f *~
