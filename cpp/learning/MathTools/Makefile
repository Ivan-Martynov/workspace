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
ALGORITHMS_MAIN_TARGET := $(BIN_DIR)/Algorithms/main
NUMBERS_MAIN_TARGET := $(BIN_DIR)/Numbers/main
STATISTICS_MAIN_TARGET := $(BIN_DIR)/Statistics/main
STRUCTURES_MAIN_TARGET := $(BIN_DIR)/Structures/main

# List of targets to make.
TARGETS := $(ALGORITHMS_MAIN_TARGET) $(NUMBERS_MAIN_TARGET) \
	$(STATISTICS_MAIN_TARGET) $(STRUCTURES_MAIN_TARGET)

.PHONY: all
all: $(TARGETS)

# Set targets' dependencies.
$(ALGORITHMS_MAIN_TARGET): $(OBJ_DIR)/Algorithms/main.o
$(NUMBERS_MAIN_TARGET): $(OBJ_DIR)/Numbers/main.o
$(STATISTICS_MAIN_TARGET): $(OBJ_DIR)/Statistics/main.o
$(STRUCTURES_MAIN_TARGET): $(OBJ_DIR)/Structures/main.o

# $@ - target, $^ - all prerequisites without duplicates.
# Rules for cpp files.
$(TARGETS):
	$(CXX) -o $@ $^ $(LDFLAGS)

include makefile_postsetup.mk


# Add prefix to folder's name for a rule to have different name.
#BUILD_PREFIX := build_folder_
#CLEAN_PREFIX := clean_folder_
#CLEAN_DEPS_PREFIX := clean_folder_deps_
#CLEAN_ALL_PREFIX := clean_folder_all_
#
## Substituting using references.
#NAMES_TO_BUILD := $(FOLDERS:%=$(BUILD_PREFIX)%)
#NAMES_TO_CLEAN := $(FOLDERS:%=$(CLEAN_PREFIX)%)
#NAMES_TO_CLEAN_DEPS := $(FOLDERS:%=$(CLEAN_DEPS_PREFIX)%)
#NAMES_TO_CLEAN_ALL := $(FOLDERS:%=$(CLEAN_ALL_PREFIX)%)
#
#all: $(NAMES_TO_BUILD)
#
## Define rules to process each folder.
#$(NAMES_TO_BUILD):
#	$(MAKE) -C $(@:$(BUILD_PREFIX)%=%)
#
## Define rules to clean each folder.
#$(NAMES_TO_CLEAN):
#	$(MAKE) clean -C $(patsubst $(CLEAN_PREFIX)%,%,$@)
#
#$(NAMES_TO_CLEAN_DEPS):
#	$(MAKE) clean_deps -C $(patsubst $(CLEAN_DEPS_PREFIX)%,%,$@)
#
#$(NAMES_TO_CLEAN_ALL):
#	$(MAKE) clean_all -C $(patsubst $(CLEAN_ALL_PREFIX)%,%,$@)
#
#.PHONY: clean clean_deps clean_all
#clean: $(NAMES_TO_CLEAN)
#clean_deps: $(NAMES_TO_CLEAN_DEPS)
#clean_all: $(NAMES_TO_CLEAN_ALL)
