# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /cygdrive/c/Users/jfgar/AppData/Local/JetBrains/CLion2020.3/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/jfgar/AppData/Local/JetBrains/CLion2020.3/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/projeto_SO/RaceSimulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/RaceSimulator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RaceSimulator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RaceSimulator.dir/flags.make

CMakeFiles/RaceSimulator.dir/main.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/RaceSimulator.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/main.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/main.c

CMakeFiles/RaceSimulator.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/main.c > CMakeFiles/RaceSimulator.dir/main.c.i

CMakeFiles/RaceSimulator.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/main.c -o CMakeFiles/RaceSimulator.dir/main.c.s

CMakeFiles/RaceSimulator.dir/RaceManager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/RaceManager.c.o: ../RaceManager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/RaceSimulator.dir/RaceManager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/RaceManager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/RaceManager.c

CMakeFiles/RaceSimulator.dir/RaceManager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/RaceManager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/RaceManager.c > CMakeFiles/RaceSimulator.dir/RaceManager.c.i

CMakeFiles/RaceSimulator.dir/RaceManager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/RaceManager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/RaceManager.c -o CMakeFiles/RaceSimulator.dir/RaceManager.c.s

CMakeFiles/RaceSimulator.dir/TeamManager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/TeamManager.c.o: ../TeamManager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/RaceSimulator.dir/TeamManager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/TeamManager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/TeamManager.c

CMakeFiles/RaceSimulator.dir/TeamManager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/TeamManager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/TeamManager.c > CMakeFiles/RaceSimulator.dir/TeamManager.c.i

CMakeFiles/RaceSimulator.dir/TeamManager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/TeamManager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/TeamManager.c -o CMakeFiles/RaceSimulator.dir/TeamManager.c.s

CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.o: ../MalfunctionManager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/MalfunctionManager.c

CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/MalfunctionManager.c > CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.i

CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/MalfunctionManager.c -o CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.s

CMakeFiles/RaceSimulator.dir/race_config_reader.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/race_config_reader.c.o: ../race_config_reader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/RaceSimulator.dir/race_config_reader.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/race_config_reader.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/race_config_reader.c

CMakeFiles/RaceSimulator.dir/race_config_reader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/race_config_reader.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/race_config_reader.c > CMakeFiles/RaceSimulator.dir/race_config_reader.c.i

CMakeFiles/RaceSimulator.dir/race_config_reader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/race_config_reader.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/race_config_reader.c -o CMakeFiles/RaceSimulator.dir/race_config_reader.c.s

CMakeFiles/RaceSimulator.dir/error_handler.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/error_handler.c.o: ../error_handler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/RaceSimulator.dir/error_handler.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/error_handler.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/error_handler.c

CMakeFiles/RaceSimulator.dir/error_handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/error_handler.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/error_handler.c > CMakeFiles/RaceSimulator.dir/error_handler.c.i

CMakeFiles/RaceSimulator.dir/error_handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/error_handler.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/error_handler.c -o CMakeFiles/RaceSimulator.dir/error_handler.c.s

CMakeFiles/RaceSimulator.dir/read_line.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/read_line.c.o: ../read_line.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/RaceSimulator.dir/read_line.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/read_line.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/read_line.c

CMakeFiles/RaceSimulator.dir/read_line.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/read_line.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/read_line.c > CMakeFiles/RaceSimulator.dir/read_line.c.i

CMakeFiles/RaceSimulator.dir/read_line.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/read_line.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/read_line.c -o CMakeFiles/RaceSimulator.dir/read_line.c.s

CMakeFiles/RaceSimulator.dir/to_float.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/to_float.c.o: ../to_float.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/RaceSimulator.dir/to_float.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/to_float.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/to_float.c

CMakeFiles/RaceSimulator.dir/to_float.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/to_float.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/to_float.c > CMakeFiles/RaceSimulator.dir/to_float.c.i

CMakeFiles/RaceSimulator.dir/to_float.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/to_float.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/to_float.c -o CMakeFiles/RaceSimulator.dir/to_float.c.s

CMakeFiles/RaceSimulator.dir/IPCManager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/IPCManager.c.o: ../IPCManager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/RaceSimulator.dir/IPCManager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/IPCManager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/IPCManager.c

CMakeFiles/RaceSimulator.dir/IPCManager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/IPCManager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/IPCManager.c > CMakeFiles/RaceSimulator.dir/IPCManager.c.i

CMakeFiles/RaceSimulator.dir/IPCManager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/IPCManager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/IPCManager.c -o CMakeFiles/RaceSimulator.dir/IPCManager.c.s

# Object files for target RaceSimulator
RaceSimulator_OBJECTS = \
"CMakeFiles/RaceSimulator.dir/main.c.o" \
"CMakeFiles/RaceSimulator.dir/RaceManager.c.o" \
"CMakeFiles/RaceSimulator.dir/TeamManager.c.o" \
"CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.o" \
"CMakeFiles/RaceSimulator.dir/race_config_reader.c.o" \
"CMakeFiles/RaceSimulator.dir/error_handler.c.o" \
"CMakeFiles/RaceSimulator.dir/read_line.c.o" \
"CMakeFiles/RaceSimulator.dir/to_float.c.o" \
"CMakeFiles/RaceSimulator.dir/IPCManager.c.o"

# External object files for target RaceSimulator
RaceSimulator_EXTERNAL_OBJECTS =

RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/main.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/RaceManager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/TeamManager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/MalfunctionManager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/race_config_reader.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/error_handler.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/read_line.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/to_float.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/IPCManager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/build.make
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C executable RaceSimulator.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RaceSimulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RaceSimulator.dir/build: RaceSimulator.exe

.PHONY : CMakeFiles/RaceSimulator.dir/build

CMakeFiles/RaceSimulator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RaceSimulator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RaceSimulator.dir/clean

CMakeFiles/RaceSimulator.dir/depend:
	cd /cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/projeto_SO/RaceSimulator /cygdrive/c/projeto_SO/RaceSimulator /cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug /cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug /cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles/RaceSimulator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RaceSimulator.dir/depend

