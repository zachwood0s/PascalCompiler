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
CMAKE_SOURCE_DIR = /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical

# Include any dependencies generated for this target.
include CMakeFiles/simple-tool.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/simple-tool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simple-tool.dir/flags.make

CMakeFiles/simple-tool.dir/src/main.cpp.o: CMakeFiles/simple-tool.dir/flags.make
CMakeFiles/simple-tool.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simple-tool.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple-tool.dir/src/main.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/main.cpp

CMakeFiles/simple-tool.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple-tool.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/main.cpp > CMakeFiles/simple-tool.dir/src/main.cpp.i

CMakeFiles/simple-tool.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple-tool.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/main.cpp -o CMakeFiles/simple-tool.dir/src/main.cpp.s

CMakeFiles/simple-tool.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/simple-tool.dir/src/main.cpp.o.requires

CMakeFiles/simple-tool.dir/src/main.cpp.o.provides: CMakeFiles/simple-tool.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/simple-tool.dir/build.make CMakeFiles/simple-tool.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/simple-tool.dir/src/main.cpp.o.provides

CMakeFiles/simple-tool.dir/src/main.cpp.o.provides.build: CMakeFiles/simple-tool.dir/src/main.cpp.o


CMakeFiles/simple-tool.dir/src/parser.cpp.o: CMakeFiles/simple-tool.dir/flags.make
CMakeFiles/simple-tool.dir/src/parser.cpp.o: src/parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/simple-tool.dir/src/parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple-tool.dir/src/parser.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/parser.cpp

CMakeFiles/simple-tool.dir/src/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple-tool.dir/src/parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/parser.cpp > CMakeFiles/simple-tool.dir/src/parser.cpp.i

CMakeFiles/simple-tool.dir/src/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple-tool.dir/src/parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/parser.cpp -o CMakeFiles/simple-tool.dir/src/parser.cpp.s

CMakeFiles/simple-tool.dir/src/parser.cpp.o.requires:

.PHONY : CMakeFiles/simple-tool.dir/src/parser.cpp.o.requires

CMakeFiles/simple-tool.dir/src/parser.cpp.o.provides: CMakeFiles/simple-tool.dir/src/parser.cpp.o.requires
	$(MAKE) -f CMakeFiles/simple-tool.dir/build.make CMakeFiles/simple-tool.dir/src/parser.cpp.o.provides.build
.PHONY : CMakeFiles/simple-tool.dir/src/parser.cpp.o.provides

CMakeFiles/simple-tool.dir/src/parser.cpp.o.provides.build: CMakeFiles/simple-tool.dir/src/parser.cpp.o


CMakeFiles/simple-tool.dir/src/lexar.cpp.o: CMakeFiles/simple-tool.dir/flags.make
CMakeFiles/simple-tool.dir/src/lexar.cpp.o: src/lexar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/simple-tool.dir/src/lexar.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simple-tool.dir/src/lexar.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/lexar.cpp

CMakeFiles/simple-tool.dir/src/lexar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple-tool.dir/src/lexar.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/lexar.cpp > CMakeFiles/simple-tool.dir/src/lexar.cpp.i

CMakeFiles/simple-tool.dir/src/lexar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple-tool.dir/src/lexar.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/lexar.cpp -o CMakeFiles/simple-tool.dir/src/lexar.cpp.s

CMakeFiles/simple-tool.dir/src/lexar.cpp.o.requires:

.PHONY : CMakeFiles/simple-tool.dir/src/lexar.cpp.o.requires

CMakeFiles/simple-tool.dir/src/lexar.cpp.o.provides: CMakeFiles/simple-tool.dir/src/lexar.cpp.o.requires
	$(MAKE) -f CMakeFiles/simple-tool.dir/build.make CMakeFiles/simple-tool.dir/src/lexar.cpp.o.provides.build
.PHONY : CMakeFiles/simple-tool.dir/src/lexar.cpp.o.provides

CMakeFiles/simple-tool.dir/src/lexar.cpp.o.provides.build: CMakeFiles/simple-tool.dir/src/lexar.cpp.o


# Object files for target simple-tool
simple__tool_OBJECTS = \
"CMakeFiles/simple-tool.dir/src/main.cpp.o" \
"CMakeFiles/simple-tool.dir/src/parser.cpp.o" \
"CMakeFiles/simple-tool.dir/src/lexar.cpp.o"

# External object files for target simple-tool
simple__tool_EXTERNAL_OBJECTS =

simple-tool: CMakeFiles/simple-tool.dir/src/main.cpp.o
simple-tool: CMakeFiles/simple-tool.dir/src/parser.cpp.o
simple-tool: CMakeFiles/simple-tool.dir/src/lexar.cpp.o
simple-tool: CMakeFiles/simple-tool.dir/build.make
simple-tool: /usr/local/lib/libLLVMSupport.a
simple-tool: /usr/local/lib/libLLVMCore.a
simple-tool: /usr/local/lib/libLLVMIRReader.a
simple-tool: /usr/local/lib/libLLVMAsmParser.a
simple-tool: /usr/local/lib/libLLVMBitReader.a
simple-tool: /usr/local/lib/libLLVMCore.a
simple-tool: /usr/local/lib/libLLVMBinaryFormat.a
simple-tool: /usr/local/lib/libLLVMSupport.a
simple-tool: /usr/local/lib/libLLVMDemangle.a
simple-tool: CMakeFiles/simple-tool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable simple-tool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple-tool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simple-tool.dir/build: simple-tool

.PHONY : CMakeFiles/simple-tool.dir/build

CMakeFiles/simple-tool.dir/requires: CMakeFiles/simple-tool.dir/src/main.cpp.o.requires
CMakeFiles/simple-tool.dir/requires: CMakeFiles/simple-tool.dir/src/parser.cpp.o.requires
CMakeFiles/simple-tool.dir/requires: CMakeFiles/simple-tool.dir/src/lexar.cpp.o.requires

.PHONY : CMakeFiles/simple-tool.dir/requires

CMakeFiles/simple-tool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simple-tool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simple-tool.dir/clean

CMakeFiles/simple-tool.dir/depend:
	cd /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles/simple-tool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simple-tool.dir/depend
