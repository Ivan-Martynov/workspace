# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/samsung/workspace/cpp/learning/design_patterns/behavioral

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build

# Include any dependencies generated for this target.
include command/CMakeFiles/command_number.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include command/CMakeFiles/command_number.dir/compiler_depend.make

# Include the progress variables for this target.
include command/CMakeFiles/command_number.dir/progress.make

# Include the compile flags for this target's objects.
include command/CMakeFiles/command_number.dir/flags.make

command/CMakeFiles/command_number.dir/src/number.cpp.o: command/CMakeFiles/command_number.dir/flags.make
command/CMakeFiles/command_number.dir/src/number.cpp.o: /home/samsung/workspace/cpp/learning/design_patterns/behavioral/command/src/number.cpp
command/CMakeFiles/command_number.dir/src/number.cpp.o: command/CMakeFiles/command_number.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object command/CMakeFiles/command_number.dir/src/number.cpp.o"
	cd /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/command && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT command/CMakeFiles/command_number.dir/src/number.cpp.o -MF CMakeFiles/command_number.dir/src/number.cpp.o.d -o CMakeFiles/command_number.dir/src/number.cpp.o -c /home/samsung/workspace/cpp/learning/design_patterns/behavioral/command/src/number.cpp

command/CMakeFiles/command_number.dir/src/number.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/command_number.dir/src/number.cpp.i"
	cd /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/command && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/samsung/workspace/cpp/learning/design_patterns/behavioral/command/src/number.cpp > CMakeFiles/command_number.dir/src/number.cpp.i

command/CMakeFiles/command_number.dir/src/number.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/command_number.dir/src/number.cpp.s"
	cd /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/command && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/samsung/workspace/cpp/learning/design_patterns/behavioral/command/src/number.cpp -o CMakeFiles/command_number.dir/src/number.cpp.s

# Object files for target command_number
command_number_OBJECTS = \
"CMakeFiles/command_number.dir/src/number.cpp.o"

# External object files for target command_number
command_number_EXTERNAL_OBJECTS =

/home/samsung/workspace/cpp/learning/design_patterns/behavioral/bin/command_number: command/CMakeFiles/command_number.dir/src/number.cpp.o
/home/samsung/workspace/cpp/learning/design_patterns/behavioral/bin/command_number: command/CMakeFiles/command_number.dir/build.make
/home/samsung/workspace/cpp/learning/design_patterns/behavioral/bin/command_number: command/CMakeFiles/command_number.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/samsung/workspace/cpp/learning/design_patterns/behavioral/bin/command_number"
	cd /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/command && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/command_number.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
command/CMakeFiles/command_number.dir/build: /home/samsung/workspace/cpp/learning/design_patterns/behavioral/bin/command_number
.PHONY : command/CMakeFiles/command_number.dir/build

command/CMakeFiles/command_number.dir/clean:
	cd /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/command && $(CMAKE_COMMAND) -P CMakeFiles/command_number.dir/cmake_clean.cmake
.PHONY : command/CMakeFiles/command_number.dir/clean

command/CMakeFiles/command_number.dir/depend:
	cd /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/samsung/workspace/cpp/learning/design_patterns/behavioral /home/samsung/workspace/cpp/learning/design_patterns/behavioral/command /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/command /home/samsung/workspace/cpp/learning/design_patterns/behavioral/build/command/CMakeFiles/command_number.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : command/CMakeFiles/command_number.dir/depend
