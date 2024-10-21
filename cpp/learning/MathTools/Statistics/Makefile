include ../makefile_presetup.mk

# List of targets to make.
TARGETS := $(BIN_DIR)/main

.PHONY: all
all: $(TARGETS)

# All targets require the bin directory to be present.
$(TARGETS): | $(BIN_DIR)

# $@ - target, $^ - all prerequisites without duplicates.
# Rules for cpp files.
$(BIN_DIR)/main: $(OBJ_DIR)/main.o
	$(CXX) -o $@ $^ $(LDFLAGS)

include ../makefile_postsetup.mk
