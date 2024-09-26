# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Include makefile with setup for compiler options.
include ../make_compiler_setup.mk

SOURCES := $(wildcard *.cpp)
TARGETS := $(SOURCES:.cpp=)
OBJECTS := $(SOURCES:.cpp=.o)

all: $(TARGETS) subdir_crash subdir_java

subdir_crash:
	$(MAKE) -C crash_course/

subdir_java:
	$(MAKE) -C intro_for_javaprogrammers/

#$(foreach target,$(TARGETS),$(eval $(call simple_source_to_object,$(target))))
$(foreach target,$(TARGETS),$(eval $(call simple_source_to_binary,$(target))))

.PHONY : clean_all clean clean_sub_crash clean_sub_java

clean:
	@-rm -f $(TARGETS) $(OBJECTS)

clean_sub_crash:
	$(MAKE) clean -C crash_course 

clean_sub_java:
	$(MAKE) clean -C intro_for_javaprogrammers 

distclean: clean
	@-rm -f *~

clean_all: clean clean_sub_crash clean_sub_java distclean 
