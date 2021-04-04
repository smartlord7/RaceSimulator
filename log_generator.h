/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#ifndef RACE_SIMULATOR_C_LOG_GENERATOR_H
#define RACE_SIMULATOR_C_LOG_GENERATOR_H

#include <semaphore.h>

//used to distinguish function input
#define I_SIMULATION_START "SIMULATION START"
#define I_SIMULATION_END "SIMULATION END"
#define I_COMMAND_RECEIVED "COMMAND RECEIVED"
#define I_COMMAND_EXCEPTION "WRONG COMMAND"
#define I_CAR_LOADED "CAR LOADED"
#define I_CAR_MALFUNCTION "CAR MALFUNCTION"
#define I_SIGNAL_RECEIVED "SIGNAL RECEIVED"
#define I_RACE_WIN "RACE WIN"

//used to distinguish function output
#define O_SIMULATION_STARTING " SIMULATOR STARTING"
#define O_SIMULATION_CLOSING " SIMULATOR CLOSING"
#define O_NEW_COMMAND_RECEIVED " NEW COMMAND RECEIVED: "
#define O_WRONG_COMMAND " WRONG COMMAND => "
#define O_CAR_LOADED " CAR LOADED => "
#define O_TEMP_NUM " X "
#define O_NEW_CAR_PROBLEM " NEW PROBLEM IN CAR"
#define O_SIGNAL " SIGNAL "
#define O_RECEIVED " RECEIVED"
#define O_CAR " CAR "
#define O_RACE_WON " WINS THE RACE"
#define O_TERMINATOR "\0"

/**
 * Generates an entry. Prints to screen and writes on log file the generated entry in a synchronized manner.
 * @param mode Mode in which the function is called.
 * @param data Additional data needed for some cases.
 */
void generate_log_entry(char * mode, void * data);

/**
 * Initialize the log file functionality.
 * @param lg_file_path Path of the log file.
 */
void log_init(char * lg_file_path, sem_t * sem);


#endif //RACE_SIMULATOR_C_LOG_GENERATOR_H