# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/kmj/my_git/prac3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kmj/my_git/prac3/build

# Include any dependencies generated for this target.
include CMakeFiles/CAM_V4L2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CAM_V4L2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CAM_V4L2.dir/flags.make

CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o: CMakeFiles/CAM_V4L2.dir/flags.make
CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o: ../src/CamWithV4L2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmj/my_git/prac3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o"
	/home/kmj/Edukit_BSP/linaro-4.9.4-gcc/bin/arm-linux-gnueabihf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o -c /home/kmj/my_git/prac3/src/CamWithV4L2.cpp

CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.i"
	/home/kmj/Edukit_BSP/linaro-4.9.4-gcc/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmj/my_git/prac3/src/CamWithV4L2.cpp > CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.i

CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.s"
	/home/kmj/Edukit_BSP/linaro-4.9.4-gcc/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmj/my_git/prac3/src/CamWithV4L2.cpp -o CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.s

CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.requires:

.PHONY : CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.requires

CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.provides: CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.requires
	$(MAKE) -f CMakeFiles/CAM_V4L2.dir/build.make CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.provides.build
.PHONY : CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.provides

CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.provides.build: CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o


CMakeFiles/CAM_V4L2.dir/src/main.cpp.o: CMakeFiles/CAM_V4L2.dir/flags.make
CMakeFiles/CAM_V4L2.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmj/my_git/prac3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CAM_V4L2.dir/src/main.cpp.o"
	/home/kmj/Edukit_BSP/linaro-4.9.4-gcc/bin/arm-linux-gnueabihf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CAM_V4L2.dir/src/main.cpp.o -c /home/kmj/my_git/prac3/src/main.cpp

CMakeFiles/CAM_V4L2.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CAM_V4L2.dir/src/main.cpp.i"
	/home/kmj/Edukit_BSP/linaro-4.9.4-gcc/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmj/my_git/prac3/src/main.cpp > CMakeFiles/CAM_V4L2.dir/src/main.cpp.i

CMakeFiles/CAM_V4L2.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CAM_V4L2.dir/src/main.cpp.s"
	/home/kmj/Edukit_BSP/linaro-4.9.4-gcc/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmj/my_git/prac3/src/main.cpp -o CMakeFiles/CAM_V4L2.dir/src/main.cpp.s

CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.requires

CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.provides: CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/CAM_V4L2.dir/build.make CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.provides

CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.provides.build: CMakeFiles/CAM_V4L2.dir/src/main.cpp.o


# Object files for target CAM_V4L2
CAM_V4L2_OBJECTS = \
"CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o" \
"CMakeFiles/CAM_V4L2.dir/src/main.cpp.o"

# External object files for target CAM_V4L2
CAM_V4L2_EXTERNAL_OBJECTS =

CAM_V4L2: CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o
CAM_V4L2: CMakeFiles/CAM_V4L2.dir/src/main.cpp.o
CAM_V4L2: CMakeFiles/CAM_V4L2.dir/build.make
CAM_V4L2: CMakeFiles/CAM_V4L2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kmj/my_git/prac3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable CAM_V4L2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CAM_V4L2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CAM_V4L2.dir/build: CAM_V4L2

.PHONY : CMakeFiles/CAM_V4L2.dir/build

CMakeFiles/CAM_V4L2.dir/requires: CMakeFiles/CAM_V4L2.dir/src/CamWithV4L2.cpp.o.requires
CMakeFiles/CAM_V4L2.dir/requires: CMakeFiles/CAM_V4L2.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/CAM_V4L2.dir/requires

CMakeFiles/CAM_V4L2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CAM_V4L2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CAM_V4L2.dir/clean

CMakeFiles/CAM_V4L2.dir/depend:
	cd /home/kmj/my_git/prac3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kmj/my_git/prac3 /home/kmj/my_git/prac3 /home/kmj/my_git/prac3/build /home/kmj/my_git/prac3/build /home/kmj/my_git/prac3/build/CMakeFiles/CAM_V4L2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CAM_V4L2.dir/depend
