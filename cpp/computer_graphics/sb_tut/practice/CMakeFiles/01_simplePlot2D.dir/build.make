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
CMAKE_SOURCE_DIR = /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice

# Include any dependencies generated for this target.
include CMakeFiles/01_simplePlot2D.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/01_simplePlot2D.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/01_simplePlot2D.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/01_simplePlot2D.dir/flags.make

CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o: CMakeFiles/01_simplePlot2D.dir/flags.make
CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o: src/01_simplePlot2D/01_simplePlot2D.cpp
CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o: CMakeFiles/01_simplePlot2D.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o -MF CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o.d -o CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o -c /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/src/01_simplePlot2D/01_simplePlot2D.cpp

CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/src/01_simplePlot2D/01_simplePlot2D.cpp > CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.i

CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/src/01_simplePlot2D/01_simplePlot2D.cpp -o CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.s

CMakeFiles/01_simplePlot2D.dir/src/glad.c.o: CMakeFiles/01_simplePlot2D.dir/flags.make
CMakeFiles/01_simplePlot2D.dir/src/glad.c.o: src/glad.c
CMakeFiles/01_simplePlot2D.dir/src/glad.c.o: CMakeFiles/01_simplePlot2D.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/01_simplePlot2D.dir/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/01_simplePlot2D.dir/src/glad.c.o -MF CMakeFiles/01_simplePlot2D.dir/src/glad.c.o.d -o CMakeFiles/01_simplePlot2D.dir/src/glad.c.o -c /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/src/glad.c

CMakeFiles/01_simplePlot2D.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/01_simplePlot2D.dir/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/src/glad.c > CMakeFiles/01_simplePlot2D.dir/src/glad.c.i

CMakeFiles/01_simplePlot2D.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/01_simplePlot2D.dir/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/src/glad.c -o CMakeFiles/01_simplePlot2D.dir/src/glad.c.s

# Object files for target 01_simplePlot2D
01_simplePlot2D_OBJECTS = \
"CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o" \
"CMakeFiles/01_simplePlot2D.dir/src/glad.c.o"

# External object files for target 01_simplePlot2D
01_simplePlot2D_EXTERNAL_OBJECTS =

bin/01_simplePlot2D: CMakeFiles/01_simplePlot2D.dir/src/01_simplePlot2D/01_simplePlot2D.cpp.o
bin/01_simplePlot2D: CMakeFiles/01_simplePlot2D.dir/src/glad.c.o
bin/01_simplePlot2D: CMakeFiles/01_simplePlot2D.dir/build.make
bin/01_simplePlot2D: CMakeFiles/01_simplePlot2D.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/01_simplePlot2D"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/01_simplePlot2D.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/01_simplePlot2D.dir/build: bin/01_simplePlot2D
.PHONY : CMakeFiles/01_simplePlot2D.dir/build

CMakeFiles/01_simplePlot2D.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/01_simplePlot2D.dir/cmake_clean.cmake
.PHONY : CMakeFiles/01_simplePlot2D.dir/clean

CMakeFiles/01_simplePlot2D.dir/depend:
	cd /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice /home/samsung/workspace/cpp/computer_graphics/sb_tut/practice/CMakeFiles/01_simplePlot2D.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/01_simplePlot2D.dir/depend
