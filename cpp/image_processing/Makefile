# Prevent make from trying to remake the makefile.
.PHONY: Makefile

include makefile_presetup.mk

# Set targets.
MAIN_TARGET := $(BIN_DIR)/main

# List of targets to make.
TARGETS := $(MAIN_TARGET)

.PHONY: all
all: $(TARGETS)

# Set targets' dependencies.
$(MAIN_TARGET): $(OBJ_DIR)/main.o \
	$(OBJ_DIR)/structures/pnm_format/pnm_image_base.o

# $@ - target, $^ - all prerequisites without duplicates.
# Rules for cpp files.
$(TARGETS):
	$(CXX) -o $@ $^ $(LDFLAGS)

include makefile_postsetup.mk
