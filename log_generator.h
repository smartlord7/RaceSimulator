#ifndef RACE_SIMULATOR_C_LOG_GENERATOR_H
#define RACE_SIMULATOR_C_LOG_GENERATOR_H

//used to distinguish function input
#define I_SIMULATION_START "SIMULATION START"
#define I_SIMULATION_END "SIMULATION END"
#define I_COMMAND_RECEIVED "COMMAND RECEIVED"
#define I_COMMAND_ERROR "WRONG COMMAND"
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

void generate_log_entry(char * mode, void * data);
void log_init(char * lg_file_path);


#endif //RACE_SIMULATOR_C_LOG_GENERATOR_H