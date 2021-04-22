cmake_minimum_required(VERSION 3.16)
project(RaceSimulator C)

set(CMAKE_C_STANDARD 11)

add_executable(RaceSimulator race_simulator.c global.h race_manager.c team_manager.c malfunction_manager.c race_config_reader.c exception_handler.c exception_handler.h read_line.c read_line.h to_float.c to_float.h ipc_manager.c race_config_t.h race_car_t.h race_team_t.h race_box_t.h shared_memory_t.h ipc_manager.h debug.h debug.c race_car_t.c race_config_t.c team_manager.h process_manager.c process_manager.h race_team_t.c log_generator.h log_generator.c strings.c strings.h priority_qeue.c priority_qeue.h)
