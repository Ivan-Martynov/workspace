# Prevent make from trying to remake the makefile.
.PHONY: Makefile

include makefile_presetup.mk

# Set targets.
#TARGETS := $(CXX_SOURCES:$(SRC_DIR)%.cpp=$(BIN_DIR)%)
TARGETS :=

HELLO_SOURCES := $(wildcard $(SRC_DIR)/01_hello/*.cpp)
HELLO_TARGETS := $(HELLO_SOURCES:$(SRC_DIR)%.cpp=$(BIN_DIR)%)
TARGETS += $(HELLO_TARGETS)

SHARING_DATA_SOURCES := $(wildcard $(SRC_DIR)/02_sharing_data/*.cpp)
SHARING_DATA_TARGETS := $(SHARING_DATA_SOURCES:$(SRC_DIR)%.cpp=$(BIN_DIR)%)
TARGETS += $(SHARING_DATA_TARGETS)

.PHONY: all
all: $(TARGETS)

define bin_rule
$(eval OBJ_FILE := $(1:$(BIN_DIR)%=$(OBJ_DIR)%.o))
$(1): $(OBJ_FILE)
endef

$(foreach target,$(HELLO_TARGETS),$(eval $(call bin_rule,$(target))))
$(foreach target,$(SHARING_DATA_TARGETS),$(eval $(call bin_rule,$(target))))

# $@ - target, $^ - all prerequisites without duplicates.
# Rules for cpp files.
$(TARGETS):
	$(CXX) -o $@ $^ $(LDFLAGS)

include makefile_postsetup.mk
