# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# All folders to process.
# FOLDERS := $(patsubst %/,%,$(wildcard */))
FOLDERS :=
#FOLDERS := Algorithms Numbers Statistics

#.PHONY: all $(NAMES_TO_BUILD) $(NAMES_TO_CLEAN) $(NAMES_TO_CLEAN_DEPS) \
#	$(NAMES_TO_CLEAN_ALL)

include makefile_presetup.mk

# Set targets.
MAIN_TARGET := $(BIN_DIR)/main

# List of targets to make.
TARGETS := $(MAIN_TARGET)

.PHONY: all
all: $(TARGETS)

# Set targets' dependencies.
$(MAIN_TARGET): $(OBJ_DIR)/main.o

# $@ - target, $^ - all prerequisites without duplicates.
# Rules for cpp files.
$(TARGETS):
	$(CXX) -o $@ $^ $(LDFLAGS)

include makefile_postsetup.mk
