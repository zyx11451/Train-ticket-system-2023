# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/zyx_train_ticket_system.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/zyx_train_ticket_system.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/zyx_train_ticket_system.dir/flags.make

CMakeFiles/zyx_train_ticket_system.dir/main.cpp.o: CMakeFiles/zyx_train_ticket_system.dir/flags.make
CMakeFiles/zyx_train_ticket_system.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/zyx_train_ticket_system.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/zyx_train_ticket_system.dir/main.cpp.o -c /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/main.cpp

CMakeFiles/zyx_train_ticket_system.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zyx_train_ticket_system.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/main.cpp > CMakeFiles/zyx_train_ticket_system.dir/main.cpp.i

CMakeFiles/zyx_train_ticket_system.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zyx_train_ticket_system.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/main.cpp -o CMakeFiles/zyx_train_ticket_system.dir/main.cpp.s

# Object files for target zyx_train_ticket_system
zyx_train_ticket_system_OBJECTS = \
"CMakeFiles/zyx_train_ticket_system.dir/main.cpp.o"

# External object files for target zyx_train_ticket_system
zyx_train_ticket_system_EXTERNAL_OBJECTS =

zyx_train_ticket_system: CMakeFiles/zyx_train_ticket_system.dir/main.cpp.o
zyx_train_ticket_system: CMakeFiles/zyx_train_ticket_system.dir/build.make
zyx_train_ticket_system: CMakeFiles/zyx_train_ticket_system.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable zyx_train_ticket_system"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zyx_train_ticket_system.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/zyx_train_ticket_system.dir/build: zyx_train_ticket_system

.PHONY : CMakeFiles/zyx_train_ticket_system.dir/build

CMakeFiles/zyx_train_ticket_system.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/zyx_train_ticket_system.dir/cmake_clean.cmake
.PHONY : CMakeFiles/zyx_train_ticket_system.dir/clean

CMakeFiles/zyx_train_ticket_system.dir/depend:
	cd /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/cmake-build-debug /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/cmake-build-debug /mnt/c/Users/123/CLionProjects/zyx_train_ticket_system/cmake-build-debug/CMakeFiles/zyx_train_ticket_system.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/zyx_train_ticket_system.dir/depend

