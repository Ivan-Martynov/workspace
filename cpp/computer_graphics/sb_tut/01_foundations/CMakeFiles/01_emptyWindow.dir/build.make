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
CMAKE_SOURCE_DIR = /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations

# Include any dependencies generated for this target.
include CMakeFiles/01_emptyWindow.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/01_emptyWindow.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/01_emptyWindow.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/01_emptyWindow.dir/flags.make

CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o: CMakeFiles/01_emptyWindow.dir/flags.make
CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o: src/01_emptyWindow/01_emptyWindow.cpp
CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o: CMakeFiles/01_emptyWindow.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o -MF CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o.d -o CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o -c /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/src/01_emptyWindow/01_emptyWindow.cpp

CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/src/01_emptyWindow/01_emptyWindow.cpp > CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.i

CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/src/01_emptyWindow/01_emptyWindow.cpp -o CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.s

CMakeFiles/01_emptyWindow.dir/src/glad.c.o: CMakeFiles/01_emptyWindow.dir/flags.make
CMakeFiles/01_emptyWindow.dir/src/glad.c.o: src/glad.c
CMakeFiles/01_emptyWindow.dir/src/glad.c.o: CMakeFiles/01_emptyWindow.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/01_emptyWindow.dir/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/01_emptyWindow.dir/src/glad.c.o -MF CMakeFiles/01_emptyWindow.dir/src/glad.c.o.d -o CMakeFiles/01_emptyWindow.dir/src/glad.c.o -c /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/src/glad.c

CMakeFiles/01_emptyWindow.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/01_emptyWindow.dir/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/src/glad.c > CMakeFiles/01_emptyWindow.dir/src/glad.c.i

CMakeFiles/01_emptyWindow.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/01_emptyWindow.dir/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/src/glad.c -o CMakeFiles/01_emptyWindow.dir/src/glad.c.s

# Object files for target 01_emptyWindow
01_emptyWindow_OBJECTS = \
"CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o" \
"CMakeFiles/01_emptyWindow.dir/src/glad.c.o"

# External object files for target 01_emptyWindow
01_emptyWindow_EXTERNAL_OBJECTS =

bin/01_emptyWindow: CMakeFiles/01_emptyWindow.dir/src/01_emptyWindow/01_emptyWindow.cpp.o
bin/01_emptyWindow: CMakeFiles/01_emptyWindow.dir/src/glad.c.o
bin/01_emptyWindow: CMakeFiles/01_emptyWindow.dir/build.make
bin/01_emptyWindow: CMakeFiles/01_emptyWindow.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/01_emptyWindow"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/01_emptyWindow.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/01_emptyWindow.dir/build: bin/01_emptyWindow
.PHONY : CMakeFiles/01_emptyWindow.dir/build

CMakeFiles/01_emptyWindow.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/01_emptyWindow.dir/cmake_clean.cmake
.PHONY : CMakeFiles/01_emptyWindow.dir/clean

CMakeFiles/01_emptyWindow.dir/depend:
	cd /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations /home/samsung/workspace/cpp/computer_graphics/sb_tut/01_foundations/CMakeFiles/01_emptyWindow.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/01_emptyWindow.dir/depend

