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
CMAKE_SOURCE_DIR = /home/samsung/workspace/cpp/learning/OpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/samsung/workspace/cpp/learning/OpenGL/build

# Include any dependencies generated for this target.
include src/shader_tools/CMakeFiles/ShaderCreator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/shader_tools/CMakeFiles/ShaderCreator.dir/compiler_depend.make

# Include the progress variables for this target.
include src/shader_tools/CMakeFiles/ShaderCreator.dir/progress.make

# Include the compile flags for this target's objects.
include src/shader_tools/CMakeFiles/ShaderCreator.dir/flags.make

src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o: src/shader_tools/CMakeFiles/ShaderCreator.dir/flags.make
src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o: /home/samsung/workspace/cpp/learning/OpenGL/src/shader_tools/ShaderCreator.cpp
src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o: src/shader_tools/CMakeFiles/ShaderCreator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/samsung/workspace/cpp/learning/OpenGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o"
	cd /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o -MF CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o.d -o CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o -c /home/samsung/workspace/cpp/learning/OpenGL/src/shader_tools/ShaderCreator.cpp

src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.i"
	cd /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/samsung/workspace/cpp/learning/OpenGL/src/shader_tools/ShaderCreator.cpp > CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.i

src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.s"
	cd /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/samsung/workspace/cpp/learning/OpenGL/src/shader_tools/ShaderCreator.cpp -o CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.s

# Object files for target ShaderCreator
ShaderCreator_OBJECTS = \
"CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o"

# External object files for target ShaderCreator
ShaderCreator_EXTERNAL_OBJECTS =

src/shader_tools/libShaderCreator.a: src/shader_tools/CMakeFiles/ShaderCreator.dir/ShaderCreator.cpp.o
src/shader_tools/libShaderCreator.a: src/shader_tools/CMakeFiles/ShaderCreator.dir/build.make
src/shader_tools/libShaderCreator.a: src/shader_tools/CMakeFiles/ShaderCreator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/samsung/workspace/cpp/learning/OpenGL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libShaderCreator.a"
	cd /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools && $(CMAKE_COMMAND) -P CMakeFiles/ShaderCreator.dir/cmake_clean_target.cmake
	cd /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ShaderCreator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/shader_tools/CMakeFiles/ShaderCreator.dir/build: src/shader_tools/libShaderCreator.a
.PHONY : src/shader_tools/CMakeFiles/ShaderCreator.dir/build

src/shader_tools/CMakeFiles/ShaderCreator.dir/clean:
	cd /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools && $(CMAKE_COMMAND) -P CMakeFiles/ShaderCreator.dir/cmake_clean.cmake
.PHONY : src/shader_tools/CMakeFiles/ShaderCreator.dir/clean

src/shader_tools/CMakeFiles/ShaderCreator.dir/depend:
	cd /home/samsung/workspace/cpp/learning/OpenGL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/samsung/workspace/cpp/learning/OpenGL /home/samsung/workspace/cpp/learning/OpenGL/src/shader_tools /home/samsung/workspace/cpp/learning/OpenGL/build /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools /home/samsung/workspace/cpp/learning/OpenGL/build/src/shader_tools/CMakeFiles/ShaderCreator.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/shader_tools/CMakeFiles/ShaderCreator.dir/depend
