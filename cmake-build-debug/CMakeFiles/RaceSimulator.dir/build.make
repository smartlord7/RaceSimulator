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

CMakeFiles/RaceSimulator.dir/race_simulator.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/race_simulator.c.o: ../race_simulator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/RaceSimulator.dir/race_simulator.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/race_simulator.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/race_simulator.c

CMakeFiles/RaceSimulator.dir/race_simulator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/race_simulator.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/race_simulator.c > CMakeFiles/RaceSimulator.dir/race_simulator.c.i

CMakeFiles/RaceSimulator.dir/race_simulator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/race_simulator.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/race_simulator.c -o CMakeFiles/RaceSimulator.dir/race_simulator.c.s

CMakeFiles/RaceSimulator.dir/race_manager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/race_manager.c.o: ../race_manager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/RaceSimulator.dir/race_manager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/race_manager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/race_manager.c

CMakeFiles/RaceSimulator.dir/race_manager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/race_manager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/race_manager.c > CMakeFiles/RaceSimulator.dir/race_manager.c.i

CMakeFiles/RaceSimulator.dir/race_manager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/race_manager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/race_manager.c -o CMakeFiles/RaceSimulator.dir/race_manager.c.s

CMakeFiles/RaceSimulator.dir/team_manager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/team_manager.c.o: ../team_manager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/RaceSimulator.dir/team_manager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/team_manager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/team_manager.c

CMakeFiles/RaceSimulator.dir/team_manager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/team_manager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/team_manager.c > CMakeFiles/RaceSimulator.dir/team_manager.c.i

CMakeFiles/RaceSimulator.dir/team_manager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/team_manager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/team_manager.c -o CMakeFiles/RaceSimulator.dir/team_manager.c.s

CMakeFiles/RaceSimulator.dir/malfunction_manager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/malfunction_manager.c.o: ../malfunction_manager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/RaceSimulator.dir/malfunction_manager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/malfunction_manager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/malfunction_manager.c

CMakeFiles/RaceSimulator.dir/malfunction_manager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/malfunction_manager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/malfunction_manager.c > CMakeFiles/RaceSimulator.dir/malfunction_manager.c.i

CMakeFiles/RaceSimulator.dir/malfunction_manager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/malfunction_manager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/malfunction_manager.c -o CMakeFiles/RaceSimulator.dir/malfunction_manager.c.s

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

CMakeFiles/RaceSimulator.dir/util/error_handler.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/util/error_handler.c.o: ../util/error_handler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/RaceSimulator.dir/util/error_handler.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/util/error_handler.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/util/error_handler.c

CMakeFiles/RaceSimulator.dir/util/error_handler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/util/error_handler.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/util/error_handler.c > CMakeFiles/RaceSimulator.dir/util/error_handler.c.i

CMakeFiles/RaceSimulator.dir/util/error_handler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/util/error_handler.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/util/error_handler.c -o CMakeFiles/RaceSimulator.dir/util/error_handler.c.s

CMakeFiles/RaceSimulator.dir/util/read_line.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/util/read_line.c.o: ../util/read_line.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/RaceSimulator.dir/util/read_line.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/util/read_line.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/util/read_line.c

CMakeFiles/RaceSimulator.dir/util/read_line.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/util/read_line.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/util/read_line.c > CMakeFiles/RaceSimulator.dir/util/read_line.c.i

CMakeFiles/RaceSimulator.dir/util/read_line.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/util/read_line.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/util/read_line.c -o CMakeFiles/RaceSimulator.dir/util/read_line.c.s

CMakeFiles/RaceSimulator.dir/util/to_float.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/util/to_float.c.o: ../util/to_float.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/RaceSimulator.dir/util/to_float.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/util/to_float.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/util/to_float.c

CMakeFiles/RaceSimulator.dir/util/to_float.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/util/to_float.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/util/to_float.c > CMakeFiles/RaceSimulator.dir/util/to_float.c.i

CMakeFiles/RaceSimulator.dir/util/to_float.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/util/to_float.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/util/to_float.c -o CMakeFiles/RaceSimulator.dir/util/to_float.c.s

CMakeFiles/RaceSimulator.dir/ipc_manager.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/ipc_manager.c.o: ../ipc_manager.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/RaceSimulator.dir/ipc_manager.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/ipc_manager.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/ipc_manager.c

CMakeFiles/RaceSimulator.dir/ipc_manager.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/ipc_manager.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/ipc_manager.c > CMakeFiles/RaceSimulator.dir/ipc_manager.c.i

CMakeFiles/RaceSimulator.dir/ipc_manager.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/ipc_manager.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/ipc_manager.c -o CMakeFiles/RaceSimulator.dir/ipc_manager.c.s

CMakeFiles/RaceSimulator.dir/util/debug.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/util/debug.c.o: ../util/debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/RaceSimulator.dir/util/debug.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/util/debug.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/util/debug.c

CMakeFiles/RaceSimulator.dir/util/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/util/debug.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/util/debug.c > CMakeFiles/RaceSimulator.dir/util/debug.c.i

CMakeFiles/RaceSimulator.dir/util/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/util/debug.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/util/debug.c -o CMakeFiles/RaceSimulator.dir/util/debug.c.s

CMakeFiles/RaceSimulator.dir/log_generator.c.o: CMakeFiles/RaceSimulator.dir/flags.make
CMakeFiles/RaceSimulator.dir/log_generator.c.o: ../log_generator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/RaceSimulator.dir/log_generator.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RaceSimulator.dir/log_generator.c.o   -c /cygdrive/c/projeto_SO/RaceSimulator/log_generator.c

CMakeFiles/RaceSimulator.dir/log_generator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RaceSimulator.dir/log_generator.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/projeto_SO/RaceSimulator/log_generator.c > CMakeFiles/RaceSimulator.dir/log_generator.c.i

CMakeFiles/RaceSimulator.dir/log_generator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RaceSimulator.dir/log_generator.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/projeto_SO/RaceSimulator/log_generator.c -o CMakeFiles/RaceSimulator.dir/log_generator.c.s

# Object files for target RaceSimulator
RaceSimulator_OBJECTS = \
"CMakeFiles/RaceSimulator.dir/race_simulator.c.o" \
"CMakeFiles/RaceSimulator.dir/race_manager.c.o" \
"CMakeFiles/RaceSimulator.dir/team_manager.c.o" \
"CMakeFiles/RaceSimulator.dir/malfunction_manager.c.o" \
"CMakeFiles/RaceSimulator.dir/race_config_reader.c.o" \
"CMakeFiles/RaceSimulator.dir/util/error_handler.c.o" \
"CMakeFiles/RaceSimulator.dir/util/read_line.c.o" \
"CMakeFiles/RaceSimulator.dir/util/to_float.c.o" \
"CMakeFiles/RaceSimulator.dir/ipc_manager.c.o" \
"CMakeFiles/RaceSimulator.dir/util/debug.c.o" \
"CMakeFiles/RaceSimulator.dir/log_generator.c.o"

# External object files for target RaceSimulator
RaceSimulator_EXTERNAL_OBJECTS =

RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/race_simulator.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/race_manager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/team_manager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/malfunction_manager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/race_config_reader.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/util/error_handler.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/util/read_line.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/util/to_float.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/ipc_manager.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/util/debug.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/log_generator.c.o
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/build.make
RaceSimulator.exe: CMakeFiles/RaceSimulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/projeto_SO/RaceSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking C executable RaceSimulator.exe"
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

