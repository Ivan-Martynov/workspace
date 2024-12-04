# Prevent make from trying to remake the makefile.
.PHONY: Makefile

include makefile_presetup.mk

LDFLAGS += -lopenal -lsndfile
# Set targets.
TARGETS := $(CXX_SOURCES:$(SRC_DIR)%.cpp=$(BIN_DIR)%)

.PHONY: all
all: $(TARGETS)

define bin_rule
$(eval OBJ_FILE := $(1:$(BIN_DIR)%=$(OBJ_DIR)%.o))
$(1): $(OBJ_FILE)
endef

$(foreach target,$(TARGETS),$(eval $(call bin_rule,$(target))))

# $@ - target, $^ - all prerequisites without duplicates.
# Rules for cpp files.
$(TARGETS):
	$(CXX) -o $@ $^ $(LDFLAGS)

include makefile_postsetup.mk
