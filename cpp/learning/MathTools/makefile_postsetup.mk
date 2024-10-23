# Define rules for each object.
define build_object_rule
# Get object's folder to use as an order-only prerequisite.
$(eval OBJ_FOLDER := $(dir $(1)))
# Dependency folder to use as an order-only prerequisite.
$(eval DEP_FOLDER := $(OBJ_FOLDER:$(OBJ_DIR)%=$(DEP_DIR)%))
# Use variables for source and dependency files for convenience.
$(eval SRC_FILE := $(1:$(OBJ_DIR)%.$(OBJ_EXT)=$(SRC_DIR)%.$(2)))
$(eval DEP_FILE := $(1:$(OBJ_DIR)%.$(OBJ_EXT)=$(DEP_DIR)%.$(DEP_EXT)))
$(eval DEP_TEMP_FILE := $(DEP_FILE:%.$(DEP_EXT)=%.$(DEP_TEMP_EXT)))
# Flags for making dependencies:
# https://gcc.gnu.org/onlinedocs/gcc-5.2.0/gcc/Preprocessor-Options.html
# -MT $(1) - Set the name of the target in the dependency file.
# -MMD - generate a dependency output file only with user header files (that is,
#	ignoring system header files) as a side-effect of the compilation process.
# -MP - add a phony target for each dependency file, except the main one. Thus
#	each dependency depends on nothing. Works for cases when header files are
#	removed without updating the Makefile to match.
# -MF $(DEP_TEMP_FILE) - determine a file to write to instead of default.
$(eval DEPFLAGS := -MT $(1) -MMD -MP -MF $(DEP_TEMP_FILE))
# Delete built-in rules.
$(1): $(SRC_FILE)
# Use the custom rule instead.
# - $(DEP_FILE) - use the dependency file as a prerequisite of the target, so
#	that if it's missing, then then target will be rebuilt.
# - | $(OBJ_FOLDER) $(DEP_FOLDER) - dependency directories as an order-only
#	prerequisites of the target.
# - Rename the temporary file with the correct dependency extension and touch to
# update the target's timestamp (that is, make knows it's newer). This trick is
# to prevent problems if a dependency file is corrupted (for example, the build
# was interrupted).
$(1): $(SRC_FILE) $(DEP_FILE) | $(OBJ_FOLDER) $(DEP_FOLDER)
	@echo Building $$@
	$(3) $(DEPFLAGS) $(4) -c -o $$@ $$<
	mv $(DEP_TEMP_FILE) $(DEP_FILE) && touch $$@
endef

# Build rule for each object.
$(foreach FILE_OBJ,$(CC_OBJECTS),$(eval $(call \
	build_object_rule,$(FILE_OBJ),$(CC_SRC_EXT),$(CC),$(CFLAGS))))
$(foreach FILE_OBJ,$(CXX_OBJECTS),$(eval $(call \
	build_object_rule,$(FILE_OBJ),$(CXX_SRC_EXT),$(CXX),$(CXXFLAGS))))

# Add folder dependencies for targets.
$(foreach TARGET,$(TARGETS),$(eval $(TARGET) : | $(dir $(TARGET))))

# Using sort function to retrieve unique folders thus avoiding duplicated rules.
$(foreach FOLDER,\
	$(sort $(dir $(CC_OBJECTS) $(CXX_OBJECTS))) \
	$(sort $(dir $(CC_DEPENDENCIES) $(CXX_DEPENDENCIES))) \
	$(sort $(dir $(TARGETS))), $(eval $(FOLDER) : ; @mkdir -p $$@))
#$(BIN_DIR), $(eval $(FOLDER) : ; @mkdir -p $$@))

# Each dependency file as a target: make won’t fail if the file doesn’t exist.
$(CC_DEPENDENCIES):
# Include the dependency files with the use of wildcard to avoid
# failing on non-existent files.
-include $(wildcard $(CC_DEPENDENCIES))

$(CXX_DEPENDENCIES):
-include $(wildcard $(CXX_DEPENDENCIES))

.PHONY: clean clean_deps clean_all
clean: ; @rm -f $(CC_OBJECTS) $(CXX_OBJECTS) $(TARGETS)
clean_deps: ; @rm -f $(CC_DEPENDENCIES) $(CXX_DEPENDENCIES)
clean_all: clean clean_deps
