cmake_minimum_required(VERSION 3.16)
project(RaceSimulator C)

set(CMAKE_C_STANDARD 11)

add_executable(RaceSimulator race_simulator.c global.h race_manager.c team_manager.c malfunction_manager.c race_config_reader.c util/error_handler.c util/error_handler.h util/read_line.c util/read_line.h util/to_float.c util/to_float.h ipc_manager.c structs/race_config_t.h structs/race_car_t.h structs/race_team_t.h structs/race_box_t.h structs/shared_memory.h ipc_manager.h util/debug.h util/debug.c structs/race_car_t.c team_manager.h util/process_manager.c util/process_manager.h structs/race_team_t.c log_generator.h log_generator.c util/str_concat.c util/str_concat.h)
