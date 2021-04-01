cmake_minimum_required(VERSION 3.16)
project(RaceSimulator C)

set(CMAKE_C_STANDARD 11)

<<<<<<< Updated upstream
add_executable(RaceSimulator main.c global_header.h RaceManager.c TeamManager.c MalfunctionManager.c)
=======
add_executable(RaceSimulator main.c race_config_reader.c error_handler.c error_handler.h read_line.c read_line.h to_float.c to_float.h)
>>>>>>> Stashed changes
