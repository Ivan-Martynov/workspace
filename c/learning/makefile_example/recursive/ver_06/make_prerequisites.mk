SOURCES := $(wildcard *.c)

include $($(SOURCES):.c=.d)

TARGETS := $(wildcard *.d)

# Ensure the recipe to be explicit, avoid possible conflicts with file names.
.PHONY: all
all: $(TARGETS)


# Generaing .d files.
%.d : %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o :, g' < $@.$$$$ > $@; \
	rm -r $@.$$$$

