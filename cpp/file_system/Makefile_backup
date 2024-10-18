# Prevent make from trying to remake the makefile.
.PHONY: Makefile

# Defining compiler.
CXX := g++

# Flags for linking and libraries.
LDFLAGS :=
LDLIBS :=

# Set name for the default target to build.
BUILD := debug

# Flags used for either debug or release.
compilation_flags.common := -std=c++23 -Wall -Wshadow -Wpedantic -Wextra \
	-Werror -Weffc++
# Debug specific flags.
compilation_flags.debug := -O0 -g -D _DEBUG
# Release specific flags.
compilation_flags.release := -O2 -D NDEBUG

# Replacing default CFLAGS with custom ones.
CXXFLAGS := ${compilation_flags.common} ${compilation_flags.${BUILD}}

# Define directories.
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin/$(BUILD)
DEP_DIR := $(OBJ_DIR)/dependencies
INC_DIR := include

CXXFLAGS += -I $(INC_DIR)

# vpath variable: search for source/header files in the src/include folder.
# That way we can omit explicit directory part for files.
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(OBJ_DIR)

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
DEPENDENCIES := $(patsubst $(SRC_DIR)/%.cpp, $(DEP_DIR)/%.d, $(SOURCES))

TARGETS := $(BIN_DIR)/show_disk_usage $(BIN_DIR)/batch_rename_files \
	$(BIN_DIR)/organize_files $(BIN_DIR)/mp3_batch_rename_files

.PHONY : all
all : $(TARGETS)

$(BIN_DIR)/batch_rename_files: $(OBJ_DIR)/batch_rename_files.o \
	$(OBJ_DIR)/BatchFileRenamer.o $(OBJ_DIR)/Utilities.o \
	$(OBJ_DIR)/FileRenameCommandBase.o $(OBJ_DIR)/FileOverwritePrompt.o \
	$(OBJ_DIR)/CaseModifyCommand.o $(OBJ_DIR)/FileNameValidator.o \
	$(OBJ_DIR)/StringAppendCommand.o $(OBJ_DIR)/StringHelper.o \
	$(OBJ_DIR)/StringReplaceCommand.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/mp3_batch_rename_files: $(OBJ_DIR)/mp3_batch_rename_files.o \
	$(OBJ_DIR)/MP3Tag.o $(OBJ_DIR)/MP3BatchFileRenamer.o \
	$(OBJ_DIR)/BatchFileRenamer.o $(OBJ_DIR)/FileRenameCommandBase.o \
	$(OBJ_DIR)/FileOverwritePrompt.o $(OBJ_DIR)/MP3Tag.o $(OBJ_DIR)/Utilities.o \
	$(OBJ_DIR)/StringAppendCommand.o $(OBJ_DIR)/StringHelper.o \
	$(OBJ_DIR)/CaseModifyCommand.o $(OBJ_DIR)/FileNameValidator.o \
	$(OBJ_DIR)/StringReplaceCommand.o $(OBJ_DIR)/SyncsafeIntConverter.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/organize_files: $(OBJ_DIR)/organize_files.o $(OBJ_DIR)/MP3Tag.o \
	$(OBJ_DIR)/MP3BatchFileRenamer.o $(OBJ_DIR)/BatchFileRenamer.o \
	$(OBJ_DIR)/FileRenameCommandBase.o $(OBJ_DIR)/FileOverwritePrompt.o \
	$(OBJ_DIR)/MP3Tag.o $(OBJ_DIR)/Utilities.o \
	$(OBJ_DIR)/StringAppendCommand.o $(OBJ_DIR)/StringHelper.o \
	$(OBJ_DIR)/CaseModifyCommand.o $(OBJ_DIR)/FileNameValidator.o \
	$(OBJ_DIR)/FileOrganizer.o $(OBJ_DIR)/StringReplaceCommand.o \
	$(OBJ_DIR)/SyncsafeIntConverter.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/show_disk_usage: $(OBJ_DIR)/show_disk_usage.o
	$(CXX) $^ -o $@ $(LDFLAGS)

# Flags for making dependencies:
# https://gcc.gnu.org/onlinedocs/gcc-5.2.0/gcc/Preprocessor-Options.html
# -MT $@ - Set the name of the target in the dependency file.
# -MMD - generate a dependency output file only with user header files (that is,
#	ignoring system header files) as a side-effect of the compilation process.
# -MP - add a phony target for each dependency file, except the main one. Thus
#	each dependency depends on nothing. Works for cases when header files are
#	removed without updating Makefile to match.
# -MF $(DEPDIR)/$*.d - determine a file to write to instead of default.
# Using .Td extension for a temporary dependency.
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td

# Build compilation commands to produce dependencies along.
COMPILE.cxx = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c -o $@

# Rename the temporary file with the correct dependency extension and touch to
# update the timestamp (that is, make knows it's newer). This trick is to
# prevent problems if a dependency file is corrupted (for example, the build
# was interrupted).
POSTCOMPILE = mv $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

# Delete built-in rules.
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
# Use the custom rule instead.
# - $(DEPDIR)/%.d - use the dependency file as a prerequisite of the target, so
#	that if it’s missing target will be rebuilt.
# - | $(DEPDIR) - dependency directory as an order-only prerequisite of the
#	target, so that it will be created when needed.
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(DEP_DIR)
	@echo Building $@ ...
	$(COMPILE.cxx) $<
	$(POSTCOMPILE)

# Directory as an order-only prerequisite. Create if none.
$(OBJECTS) : | $(OBJ_DIR)
$(OBJ_DIR) : ; mkdir $@

# Directory as an order-only prerequisite. Create if none.
$(TARGETS) : | $(BIN_DIR)
$(BIN_DIR) : ; mkdir -p $@

# Directory as an order-only prerequisite. Create if none.
$(DEP_DIR): ; mkdir -p $@
# Each dependency file as a target: make won’t fail if the file doesn’t exist.
$(DEPENDENCIES):
# Include the dependency files with the use of wildcard to avoid
# failing on non-existent files.
-include $(wildcard $(DEPENDENCIES))

.PHONY : clean clean_dependencies clean_all

clean_dependencies: ; -rm -f $(DEPENDENCIES)

clean : ; -rm -f $(OBJECTS) $(TARGETS)

clean_all: clean_dependencies clean
