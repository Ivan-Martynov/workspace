# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build

# Include any dependencies generated for this target.
include CMakeFiles/05_geometryTessTriangle.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/05_geometryTessTriangle.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/05_geometryTessTriangle.dir/flags.make

CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o: CMakeFiles/05_geometryTessTriangle.dir/flags.make
CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o: ../src/05_geometryTessTriangle/05_geometryTessTriangle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o -c /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp

CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp > CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.i

CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp -o CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.s

CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.requires:

.PHONY : CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.requires

CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.provides: CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.requires
	$(MAKE) -f CMakeFiles/05_geometryTessTriangle.dir/build.make CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.provides.build
.PHONY : CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.provides

CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.provides.build: CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o


CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o: CMakeFiles/05_geometryTessTriangle.dir/flags.make
CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o: ../src/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o   -c /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/src/glad.c

CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/src/glad.c > CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.i

CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/src/glad.c -o CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.s

CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.requires:

.PHONY : CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.requires

CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.provides: CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.requires
	$(MAKE) -f CMakeFiles/05_geometryTessTriangle.dir/build.make CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.provides.build
.PHONY : CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.provides

CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.provides.build: CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o


# Object files for target 05_geometryTessTriangle
05_geometryTessTriangle_OBJECTS = \
"CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o" \
"CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o"

# External object files for target 05_geometryTessTriangle
05_geometryTessTriangle_EXTERNAL_OBJECTS =

../bin/05_geometryTessTriangle: CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o
../bin/05_geometryTessTriangle: CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o
../bin/05_geometryTessTriangle: CMakeFiles/05_geometryTessTriangle.dir/build.make
../bin/05_geometryTessTriangle: CMakeFiles/05_geometryTessTriangle.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/05_geometryTessTriangle"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/05_geometryTessTriangle.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/05_geometryTessTriangle.dir/build: ../bin/05_geometryTessTriangle

.PHONY : CMakeFiles/05_geometryTessTriangle.dir/build

CMakeFiles/05_geometryTessTriangle.dir/requires: CMakeFiles/05_geometryTessTriangle.dir/src/05_geometryTessTriangle/05_geometryTessTriangle.cpp.o.requires
CMakeFiles/05_geometryTessTriangle.dir/requires: CMakeFiles/05_geometryTessTriangle.dir/src/glad.c.o.requires

.PHONY : CMakeFiles/05_geometryTessTriangle.dir/requires

CMakeFiles/05_geometryTessTriangle.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/05_geometryTessTriangle.dir/cmake_clean.cmake
.PHONY : CMakeFiles/05_geometryTessTriangle.dir/clean

CMakeFiles/05_geometryTessTriangle.dir/depend:
	cd /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build /home/paaanda/Dropbox/workspace/cpp/computer_graphics/sb_tut/01_starting/build/CMakeFiles/05_geometryTessTriangle.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/05_geometryTessTriangle.dir/depend

