//
// Created by jfgar on 02/04/2021.
//

#ifndef RACE_SIMULATOR_C_LOG_GENERATOR_H
#define RACE_SIMULATOR_C_LOG_GENERATOR_H

#define SIMULATION_START 0
#define SIMULATION_END 1
#define COMMAND_RECEIVED 2
#define COMMAND_ERROR 3
#define CAR_LOADED 4
#define CAR_MALFUNCTION 5
#define SIGNAL_RECEIVED 6
#define RACE_WIN 7

void generate_log_entry(int mode, void * data);
void log_init(char * lg_file_path);


#endif //RACE_SIMULATOR_C_LOG_GENERATOR_H
