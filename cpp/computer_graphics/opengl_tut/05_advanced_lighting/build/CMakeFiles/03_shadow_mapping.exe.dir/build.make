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
CMAKE_SOURCE_DIR = /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/build

# Include any dependencies generated for this target.
include CMakeFiles/03_shadow_mapping.exe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/03_shadow_mapping.exe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/03_shadow_mapping.exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/03_shadow_mapping.exe.dir/flags.make

CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o: CMakeFiles/03_shadow_mapping.exe.dir/flags.make
CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o: /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/03_shadow_mapping/main.cpp
CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o: CMakeFiles/03_shadow_mapping.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o -MF CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o.d -o CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o -c /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/03_shadow_mapping/main.cpp

CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/03_shadow_mapping/main.cpp > CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.i

CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/03_shadow_mapping/main.cpp -o CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.s

# Object files for target 03_shadow_mapping.exe
03_shadow_mapping_exe_OBJECTS = \
"CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o"

# External object files for target 03_shadow_mapping.exe
03_shadow_mapping_exe_EXTERNAL_OBJECTS =

/home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/bin/03_shadow_mapping.exe: CMakeFiles/03_shadow_mapping.exe.dir/03_shadow_mapping/main.cpp.o
/home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/bin/03_shadow_mapping.exe: CMakeFiles/03_shadow_mapping.exe.dir/build.make
/home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/bin/03_shadow_mapping.exe: CMakeFiles/03_shadow_mapping.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/bin/03_shadow_mapping.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/03_shadow_mapping.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/03_shadow_mapping.exe.dir/build: /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/bin/03_shadow_mapping.exe
.PHONY : CMakeFiles/03_shadow_mapping.exe.dir/build

CMakeFiles/03_shadow_mapping.exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/03_shadow_mapping.exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/03_shadow_mapping.exe.dir/clean

CMakeFiles/03_shadow_mapping.exe.dir/depend:
	cd /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/build /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/build /home/samsung/workspace/cpp/computer_graphics/opengl_tut/05_advanced_lighting/build/CMakeFiles/03_shadow_mapping.exe.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/03_shadow_mapping.exe.dir/depend

