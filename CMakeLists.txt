cmake_minimum_required(VERSION 3.16)
project(RaceSimulator C)

set(CMAKE_C_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(RaceSimulator processes/race_simulator/race_simulator.c util/global.h processes/race_manager/race_manager.c processes/team_manager/team_manager.c processes/malfunction_manager/malfunction_manager.c util/race_config_reader/race_config_reader.c util/debug/exception_handler.c util/debug/exception_handler.h util/read_line/read_line.c util/read_line/read_line.h util/to_float/to_float.c util/to_float/to_float.h structs/race_config/race_config_t.h structs/race_car/race_car_t.h structs/race_team/race_team_t.h structs/race_box/race_box_t.h structs/shared_memory/shared_memory_t.h util/debug/debug.h util/debug/debug.c structs/race_car/race_car_t.c structs/race_config/race_config_t.c processes/team_manager/team_manager.h util/process_manager/process_manager.c util/process_manager/process_manager.h structs/race_team/race_team_t.c util/log_generator/log_generator.h util/log_generator/log_generator.c util/strings/strings.c util/strings/strings.h priority_queue.c priority_queue.h sync_t.h ipcs/message_queue/msg_queue.c ipcs/message_queue/msg_queue.h ipcs/pipe/pipe.c ipcs/pipe/pipe.h ipcs/shared_memory/shm.c ipcs/shared_memory/shm.h ipcs/sync/mutex/mutex.c ipcs/sync/mutex/mutex.h ipcs/sync/condition_variable/cond_var.c ipcs/sync/condition_variable/cond_var.h ipcs/sync/semaphore/sem.h ipcs/sync/semaphore/sem.c)
